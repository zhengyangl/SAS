#include <fstream>
#include "BlackWhiteListCheckerDisabler.h"

BlackWhiteListCheckerDisabler::BlackWhiteListCheckerDisabler(const clang::Decl *D,
                                                             const char *checker,
                                                             clang::ento::BugReporter &BR,
                                                             const clang::SourceLocation &sLoc)
   : sLoc(sLoc), checker(checker)
{
   auto &mgr = BR.getSourceManager();
   relPath = mgr.getFilename(sLoc);
   auto *DC = D->getDeclContext();
   WalkDeclContext(DC);
}

BlackWhiteListCheckerDisabler::BlackWhiteListCheckerDisabler(clang::ento::CheckerContext &C,
                                                             const char *checker,
                                                             clang::ento::BugReporter &BR,
                                                             const clang::SourceLocation &sLoc)
   : sLoc(sLoc), checker(checker)
{
   auto &mgr = BR.getSourceManager();
   relPath = mgr.getFilename(sLoc);
   const clang::LocationContext *LC = C.getLocationContext();
   auto *D = LC->getDecl();
   auto *DC = D->getDeclContext();
   WalkDeclContext(DC);
}

void BlackWhiteListCheckerDisabler::WalkDeclContext(const clang::DeclContext *DC){
   auto *enclosingNamespaceContext = DC->getEnclosingNamespaceContext();

   if(enclosingNamespaceContext->isNamespace())
   {
      auto *NNDC = clang::NamespaceDecl::castFromDeclContext(enclosingNamespaceContext);
      namespace_name = NNDC->getQualifiedNameAsString();
   }
   auto *traversalContext = DC;
   while(!traversalContext->isTranslationUnit())
   {
      if(!traversalContext->isRecord()){
         traversalContext = traversalContext->getParent();
         continue;
      }
      const clang::RecordDecl *RD = clang::cast<clang::RecordDecl>(traversalContext);
      if(RD->isClass())
         class_name = RD->getNameAsString();
      else if(RD->isStruct())
         struct_name = RD->getNameAsString();
      if((!struct_name.empty()) && (!class_name.empty())) break;
      traversalContext = traversalContext->getParent();
   }
}

bool BlackWhiteListCheckerDisabler::PathWildCardMatch(const YAML::Node &pathWildCardList,
                                                      const std::string &matchString) const {
   if(matchString.empty()) return true;
   if(pathWildCardList && pathWildCardList.Type() == YAML::NodeType::Sequence)
   {
      for(std::size_t listIndex = 0;
          listIndex < pathWildCardList.size();
          ++listIndex){
         if (fnmatch(pathWildCardList[listIndex].as<std::string>().c_str(),
                     matchString.c_str(),
                     FNM_PATHNAME) == 0)
            return true;
         }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::RegexMatch(const YAML::Node &regexList,
                                               const std::string &matchString) const {
   if(matchString.empty()) return true;
   if(regexList && regexList.Type() == YAML::NodeType::Sequence)
   {
      for(std::size_t regexListIndex = 0;
          regexListIndex < regexList.size();
          ++regexListIndex){
         std::regex regex(regexList[regexListIndex].as<std::string>());
         if(std::regex_match(matchString, regex))
            return true;
      }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::PrefixMatch(const YAML::Node &prefixList,
                                                const std::string &matchString) const{
   if(matchString.empty()) return true;
   if(prefixList && prefixList.Type() == YAML::NodeType::Sequence)
   {
      for(std::size_t prefixListIndex = 0;
          prefixListIndex < prefixList.size();
          ++prefixListIndex)
      {
         std::string prefixInList = prefixList[prefixListIndex].as<std::string>();
         if(std::equal(prefixInList.begin(), prefixInList.end(), matchString.begin()))
            return true;
      }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::ParseBlackWhiteField(const YAML::Node &bwConfig) const{
   if(bwConfig && bwConfig.Type() == YAML::NodeType::Scalar){
      bool bw = (bwConfig.as<std::string>() == "W") ? true : false;
         return bw;
   }
   return false;
}

bool BlackWhiteListCheckerDisabler::CheckBlackWhiteListConfiguration() const{
   // scan if SA_CONFIGURATION environment variable is set
   std::string sas_configuration_path;
   if(const char* env_p = std::getenv("SA_CONFIGURATION"))
   {
      sas_configuration_path = env_p;
   }
   // if SA_CONFIGURATION is not set we will scan the .sas.yaml under working directory
   // if we cannot find configuration yaml, then we fire the checker by default
   else sas_configuration_path = ".sas.yaml";
   std::ifstream config_file(sas_configuration_path);
   if (!config_file.good()) {
        config_file.close();
        return false;
   }
   else config_file.close();
   YAML::Node config = YAML::LoadFile(sas_configuration_path)["CONFIGURATION"];

   if(!config) return false;

   // parse the configuration file
   for (std::size_t config_i = 0; config_i < config.size(); ++config_i) {
      bool path_match=PathWildCardMatch(config[config_i]["FILE_PATH"], relPath.str()),
         namespace_match=RegexMatch(config[config_i]["NAMESPACE"], namespace_name),
         struct_match=RegexMatch(config[config_i]["STRUCT"], struct_name),
         class_match=RegexMatch(config[config_i]["CLASS"], class_name);

      bool match = path_match && namespace_match && struct_match && class_match;
      //if all path/namespace/class/struct are matched, we then check the b/w and decide whether we fire
      if (match)
      {
         bool blackWhite = ParseBlackWhiteField(config[config_i]["B/W"]),
            checkerMatch = PrefixMatch(config[config_i]["CHECKER"], checker),
            ifLaunch = (checkerMatch) ? blackWhite : !blackWhite;
         if(!ifLaunch) return true;
      }
   }
   return false;
}

bool BlackWhiteListCheckerDisabler::isDisabled()
{
   return CheckBlackWhiteListConfiguration();
}
