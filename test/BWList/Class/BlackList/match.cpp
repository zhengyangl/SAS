class TApple
{
   typedef int time;  // RN3 bug
   typedef int time_t;
   enum EmyEnum{};
   enum myEnum{}; //RN6 bug
};
class TBoy
{
   typedef int time;  // RN3 bug
   typedef int time_t;
   enum EmyEnum{};
   enum myEnum{}; //RN6 bug
};
