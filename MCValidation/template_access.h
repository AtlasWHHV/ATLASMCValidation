#ifndef __template_access__
#define __template_access__

#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/tools/ReturnCheck.h"
#include <string>

// Get rid of these once things sort-of work
using namespace std;
using namespace xAOD;

// Some methods to see if we can't make plotting simple things... simple.

template <class T>
class toplevel_looper_impl {
 public:
  toplevel_looper_impl (const string &containerName)
    : _containerName(containerName)
  {
    cout << "Looking at " << _containerName << endl;
  }

  // Call after the event has been advanced to the proper place to take a look.
  int ProcessEvents (TEvent &event)
  {
    const T *container = nullptr;
    RETURN_CHECK ("bogus", event.retrieve(container, _containerName.c_str()));

    return 0;
  }

 private:
  const string &_containerName;

};

// Create a looper object from a raw event (just to make life easy)
template <class T>
toplevel_looper_impl<T> looper(const string &containerName)
{
  return toplevel_looper_impl<T>(containerName);
}

#endif
