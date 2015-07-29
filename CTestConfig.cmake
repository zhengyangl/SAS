cmake_minimum_required(VERSION 2.8)
######################################################
# CDash drop site settings
set(CTEST_PROJECT_NAME "SAS")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "cdash.cern.ch")
set(CTEST_DROP_LOCATION "/submit.php?project=SAS")
set(CTEST_DROP_SITE_CDASH TRUE)
