//
//  main.swift
//  BTT
//
//  Created by Leif Wen on 2020/9/10.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

import Foundation
import IOBluetooth
import Cocoa


@_cdecl("Swift_MainLoop")
func MainLoop(){
	while(SDT_IsExit() == 0) {
		BTManager?.Run()
		RunLoop.current.run(mode: RunLoop.Mode.default, before: Date.init(timeIntervalSinceNow: 0.1))
	}
	BTManager?.Exit()
}


let argc = CommandLine.argc
let argv = CommandLine.arguments
var cargv = argv.map { strdup($0) } 	// Create [UnsafeMutablePointer<Int8>]:
let result = _main(argc, &cargv) 		// Call main(int argc,char *argv[]) function:
for ptr in cargv { free(ptr) }			// Free the duplicated strings:
