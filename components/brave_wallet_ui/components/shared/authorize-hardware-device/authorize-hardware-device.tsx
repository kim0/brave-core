/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { StyledIFrame } from './style'
import { LEDGER_BRIDGE_URL } from '../../../common/hardware/ledgerjs/ledger-messages'

export const AuthorizeHardwareDeviceIFrame = () => {
  const src = LEDGER_BRIDGE_URL + `?targetUrl=${encodeURIComponent(window.origin)}`
  return (
    <StyledIFrame src={src} allow="hid" frameBorder="0"/>
  )
}

export default AuthorizeHardwareDeviceIFrame
