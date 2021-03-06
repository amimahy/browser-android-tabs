// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  await TestRunner.setupStartupTest('resources/console-clear-arguments-on-frame-navigation.html');
  TestRunner.addResult(
      `Tests that Web Inspector will discard console message arguments and convert first argument into a string when iframe where the message was logged is navigated to a different page.\n`);
  await TestRunner.loadModule('console_test_runner');

  for (var message of ConsoleModel.consoleModel.messages()) {
    var args = (message.parameters || []).map((arg) => arg.type);
    TestRunner.addResult('Message: "' + message.messageText + '", arguments: [' + args.join(', ') + ']');
  }
  TestRunner.completeTest();
})();
