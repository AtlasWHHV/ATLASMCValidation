// Some helpers for dealing with the truth stuff
#ifndef __truth_helpers__
#define __truth_helpers__

#include "xAODTruth/TruthEvent.h"

class truth_iter {
 public:
  // create an iterator that starts at zero and goes for the full list by default.
  truth_iter(const xAOD::TruthEvent *container, int initialPosition=0)
   : _position(initialPosition), _event(container)
    {
      _lastIndex = _event->nTruthParticles();
    }

  bool operator!= (const truth_iter &other) const {
    return _position != other._position;
  }

  const xAOD::TruthParticle *operator* () const {
    return _event->truthParticle(_position);
  }

  const truth_iter& operator++ ()
  {
    const xAOD::TruthParticle* v = nullptr;
    while (v == nullptr && _position < _lastIndex) {
      _position++;
      if (_position < _lastIndex)
	v = _event->truthParticle(_position);
    }
    return *this;
  }

 private:
  int _position;
  const xAOD::TruthEvent *_event;
  int _lastIndex;
};

class truth_as_range {
 public:
  truth_as_range (const xAOD::TruthEvent *container)
    : _event(container)
  {}

  truth_iter begin() const {
    return truth_iter(_event);
  }
  truth_iter end() const {
    return truth_iter(_event, _event->nTruthParticles());
  }

 private:
  const xAOD::TruthEvent *_event;
};
#endif
