// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#ifndef MEDIAPIPE_DEPS_REGISTRATION_TOKEN_H_
#define MEDIAPIPE_DEPS_REGISTRATION_TOKEN_H_

#include <functional>
#include <vector>

namespace mediapipe {
// RegistrationToken is a generic class that represents a registration that
// can be later undone, via a call to Unregister().
//
// It is generally a good idea for registration methods, such as
// RegisterListener(X) to return ways to undo the registration (for instance if
// X goes out of scope).
// RegistrationToken is a good candidate as a return value for those methods.
//
// Example usage:
//
// RegistrationToken token = MyCancellableRegisterListener(foo);
// ...
// do something
//
// token.Unregister();
//
//
// There is also a Unregister RAII helper below that automatically unregisters
// a token when it goes out of scope:
//
// {
//   Unregister unregisterer(MyCancellableRegisterListener(foo));
//   ...
//   do something
//
// }  // unregisterer goes out of scope, we are unregistered.
//
//
// Implementation: tokens are generic, they just accept a std::function<void()>
// that does the actual unregistration. It is up to each registration system to
// pass the function that corresponds to their own implementation for
// unregistering things.
//
// In that regard, tokens are basically a glorified unique_ptr<function>.
// The main advantage is that they guarantee the function can be called only
// once, and naming is also much clearer (Unregister versus operator()).
//
// Tokens are not copyable but they are movable, which reflects the fact that
// there should only ever be one token in charge of a particular registration
// at any time (else there could be confusion, who is in charge of
// unregistering).
//
// This class is thread compatible.
class RegistrationToken {
 public:
  explicit RegistrationToken(std::function<void()> unregisterer);

  // It is useful to have an empty constructor for when we want to declare a
  // token, and assign it later.
  RegistrationToken() {}

  RegistrationToken(const RegistrationToken&) = delete;
  RegistrationToken& operator=(const RegistrationToken&) = delete;

  RegistrationToken(RegistrationToken&& rhs);
  RegistrationToken& operator=(RegistrationToken&& rhs);

  // Unregisters the registration for which this token is in charge, and voids
  // the token. It is safe to call this more than once, but further calls are
  // guaranteed to be noop.
  void Unregister();

  // Returns a token whose Unregister() will Unregister() all <tokens>.
  static RegistrationToken Combine(std::vector<RegistrationToken> tokens);

 private:
  std::function<void()> unregister_function_ = nullptr;
};

// RAII class for registration tokens: it calls Unregister() when it goes out
// of scope.
class Unregister {
 public:
  // Useful to have an empty constructor for when we want to assign it later.
  // The default is an empty token that does nothing.
  Unregister() : token_() {}
  explicit Unregister(RegistrationToken token);
  ~Unregister();

  Unregister(const Unregister&) = delete;
  Unregister& operator=(const Unregister&) = delete;

  Unregister(Unregister&& rhs);
  Unregister& operator=(Unregister&& rhs);

  // Similar to unique_ptr.reset() and the likes: this will unregister the
  // current token if any, and then assume registration ownership of this new
  // <token>.
  void Reset(RegistrationToken token = RegistrationToken());

 private:
  RegistrationToken token_;
};
}  // namespace mediapipe

#endif  // MEDIAPIPE_DEPS_REGISTRATION_TOKEN_H_
