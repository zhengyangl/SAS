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

bool BlackWhiteListCheckerDisabler::PathWildCardMatch(jsonxx::Array &pathWildCardList,
                                                      const std::string &matchString) const {
   if(matchString.empty()) return true;
   std::cout<<"PATHSIZE "<<pathWildCardList.size()<<std::endl;
   if(pathWildCardList.size() != 0)
   {
      for(std::size_t listIndex = 0;
          listIndex < pathWildCardList.size();
          ++listIndex){
         if (fnmatch(pathWildCardList.get<std::string>(listIndex).c_str(),
                     matchString.c_str(),
                     FNM_PATHNAME) == 0)
            return true;
         }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::RegexMatch(jsonxx::Array &regexList,
                                               const std::string &matchString) const {
   if(matchString.empty()) return true;
   if(regexList.size() != 0)
   {
      for(std::size_t regexListIndex = 0;
          regexListIndex < regexList.size();
          ++regexListIndex){
         std::regex regex(regexList.get<std::string>(regexListIndex));
         if(std::regex_match(matchString, regex))
            return true;
      }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::PrefixMatch(jsonxx::Array &prefixList,
                                                const std::string &matchString) const{
   if(matchString.empty()) return true;
   if(prefixList.size() != 0)
   {
      for(std::size_t prefixListIndex = 0;
          prefixListIndex < prefixList.size();
          ++prefixListIndex)
      {
         std::string prefixInList = prefixList.get<std::string>(prefixListIndex);
         if(std::equal(prefixInList.begin(), prefixInList.end(), matchString.begin()))
            return true;
      }
      return false;
   }
   else return true;
}

bool BlackWhiteListCheckerDisabler::ParseBlackWhiteField(std::string &bwConfig) const{
   if(!bwConfig.empty()){
      bool bw = (bwConfig == "W") ? true : false;
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
   // if SA_CONFIGURATION is not set we will scan the .sas.json under working directory
   // if we cannot find configuration json, then we fire the checker by default
   else sas_configuration_path = ".sas.json";
   std::ifstream config_file(sas_configuration_path);
   if (!config_file.good()) {
        config_file.close();
        return false;
   }

   std::string config_string((std::istreambuf_iterator<char>(config_file)),
                             std::istreambuf_iterator<char>());

   config_file.close();

   jsonxx::Object json;
   if(!json.parse(config_string)) return false;
   if(!json.has<jsonxx::Array>("CONFIGURATION")) return false;
   jsonxx::Array configuration_list = json.get<jsonxx::Array>("CONFIGURATION");

   for(size_t i = 0 ; i < configuration_list.size(); i ++)
   {
      jsonxx::Object configuration = configuration_list.get<jsonxx::Object>(i);
      jsonxx::Array filepathList, namespaceList, structList, classList, checkerList;

      if(configuration.has<jsonxx::Array>("FILE_PATH"))
         filepathList = configuration.get<jsonxx::Array>("FILE_PATH");
      if(configuration.has<jsonxx::Array>("NAMESPACE"))
         namespaceList = configuration.get<jsonxx::Array>("NAMESPACE");
      if(configuration.has<jsonxx::Array>("STRUCT"))
         structList = configuration.get<jsonxx::Array>("STRUCT");
      if(configuration.has<jsonxx::Array>("CLASS"))
         classList = configuration.get<jsonxx::Array>("CLASS");
      if(configuration.has<jsonxx::Array>("CHECKER"))
         checkerList = configuration.get<jsonxx::Array>("CHECKER");
      bool path_match=PathWildCardMatch(filepathList, relPath.str()),
         namespace_match=RegexMatch(namespaceList, namespace_name),
         struct_match=RegexMatch(structList, struct_name),
         class_match=RegexMatch(classList, class_name);

      bool match = path_match && namespace_match && struct_match && class_match;
      //if all path/namespace/class/struct are matched, we then check the b/w and decide whether we fire
      if (match)
      {
         bool blackWhite = configuration.get<jsonxx::Boolean>("B/W"),
            checkerMatch = PrefixMatch(checkerList, checker),
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
