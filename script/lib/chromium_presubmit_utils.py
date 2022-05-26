# Copyright (c) 2022 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/. */

import contextlib
import types


def override_check(scope, check_name=None):
    """Replaces existing PRESUBMIT checks. Can be used with globals()
    scope or a class scope (such as input_api.canned_checks)."""
    def decorator(new_func):
        is_dict_scope = isinstance(scope, dict)
        nonlocal check_name
        check_name = check_name or new_func.__name__
        if is_dict_scope:
            original_check = scope.get(check_name)
        else:
            original_check = getattr(scope, check_name)

        if not callable(original_check):
            raise NameError(f'Failed to override presubmit check: '
                            f'{check_name} not found or not callable')

        def wrapped_f(*args, **kwargs):
            return new_func(original_check, *args, **kwargs)

        if is_dict_scope:
            scope[check_name] = wrapped_f
        else:
            setattr(scope, check_name, wrapped_f)

        return wrapped_f

    return decorator


def get_first_check_name(scope):
    """Returns first Check* method from the scope."""
    assert isinstance(scope, dict)
    for key, value in scope.items():
        if key.startswith('Check') and callable(value):
            return key
    return None


@contextlib.contextmanager
def override_class_method(clazz, new_method, method_name=None):
    """Scoped class method override helper."""
    method_name = method_name or new_method.__name__
    original_method = getattr(clazz, method_name)
    try:
        if not callable(original_method):
            raise NameError(f'Failed to override class method: '
                            f'{original_method} not found or not callable')

        def wrapped_f(self, *args, **kwargs):
            return new_method(self, original_method, *args, **kwargs)

        setattr(clazz, method_name, types.MethodType(wrapped_f, clazz))
        yield
    finally:
        if original_method:
            setattr(clazz, method_name, original_method)
