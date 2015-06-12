namespace outside{
   namespace inside{
      typedef int time;  // RN3 bug
      typedef int time_t;
      class TMyClass{};
      class Tyourclass{}; //RN4 bug
      class myClass{}; // RN4 bug
   }
}

namespace wrong_outside{
   namespace inside{
      typedef int time;  // RN3 bug
      typedef int time_t;
      class TMyClass{};
      class Tyourclass{}; //RN4 bug
      class myClass{}; // RN4 bug
   }
}
