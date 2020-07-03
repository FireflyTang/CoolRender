//
//  CoolCGDelegate.h
//  Lession10
//
//  Created by 唐如麟 on 15/5/22.
//  Copyright (c) 2015年 唐如麟. All rights reserved.
//

#ifndef Lession10_CoolCGDelegate_h
#define Lession10_CoolCGDelegate_h

CC::classControl	 classControl;

void DelagatekeyRotateByCenter(int key,int,int){
	classControl.keyRotateByCenter(key,NULL,NULL);
}

void DelagatekeyRotateBySelf(int key,int,int){
	classControl.keyRotateBySelf(key,NULL,NULL);
}

void DelegatekeyMove(unsigned char key,int,int){
	classControl.keyMove(key, NULL, NULL);
}

void DelegateMousePress(int button,int state,int x,int y){
	classControl.MousePress(button, state, x, y);
}

void DelegateMouseMoveBySelf(int x,int y){
	classControl.MouseMoveBySelf(x, y);
}

void DelegateMouseMoveByCenter(int x,int y){
	classControl.MouseMoveByCenter(x, y);
}

#endif
