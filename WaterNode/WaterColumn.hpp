/*
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016 davidejones88
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal 
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
/*
 *  Original code from https://github.com/mrMakaronka/ios-2d-water-simulation
 */
#pragma once

#include "cocos2d.h"

/*
 *  This class is basically a container that holds the information about the height the target height and the speed of a single vertical slice of the water surface
 *  This Node won't be added to any Scene, and might actually be worth to convert this into a struct instead of a class
 *  Many WaterColumns together compose a water surface (WaterNode)
 */
class WaterColumn : public cocos2d::Node
{
public:
    // creation
    static WaterColumn *create()
    {
        // we call our custom creation method inline, BEWARE THAT WE DO NOT CALL THE AUTORELEASE since we want this node to stay alive even if not added to any parent
        auto ret = new WaterColumn();
        ret->init();
        return ret;
    }
    
    // variables
    float targetHeight;   // the height we want to reach, based on the splash effect we are applying
    float height;         // the real height we reached so far
    float speed;          // the speed at which we try to reach the targetHeight
    
    // functions
    bool init();
    void initWithTargetHeight(float targetHeight, float height, float speed);
    void update(float dampening, float tension);
};