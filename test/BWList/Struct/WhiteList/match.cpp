struct TApple
{
   typedef int time;  // RN3 bug
   typedef int time_t;
   enum EmyEnum{};
   enum myEnum{}; //RN6 bug
};
struct TBoy
{
   typedef int time;  // RN3 bug
   typedef int time_t;
   enum EmyEnum{};
   enum myEnum{}; //RN6 bug
};
