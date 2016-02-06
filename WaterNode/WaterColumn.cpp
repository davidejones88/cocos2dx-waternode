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
#include "WaterColumn.hpp"

using namespace cocos2d;

bool WaterColumn::init()
{
    // super init
    Node::init();
    
    return true;
}

void WaterColumn::initWithTargetHeight(float targetHeight, float height, float speed)
{
    this->targetHeight = targetHeight;
    this->height = height;
    this->speed = speed;
}

void WaterColumn::update(float dampening, float tension)
{
    float deltaHeight = targetHeight - height;
    speed += tension * deltaHeight - speed * dampening;
    height += speed;
}