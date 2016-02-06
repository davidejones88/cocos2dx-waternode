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

// define the constants that are going to define the water surface behaviour
static const int kColumnCount = 80;
static const int kWaterHeight = 150;
static const float kTension = 0.025;
static const float kDampening = 0.025;
static const float kSpread = 0.25;
static cocos2d::Color4B topColor = cocos2d::Color4B(0, 50, 200, 255);
static cocos2d::Color4B bottomColor = cocos2d::Color4B(0, 50, 100, 100);

// define a struct for Vertex we are going to use to draw primitives
struct Vertex
{
    uint16_t x;
    uint16_t y;
};

// define a struct for Color we are going to use to draw primitives
struct Color
{
    cocos2d::Color4B color;
};

// forward declarations
class WaterColumn;

/*
 *   This is the main and only node you need to actually create in order to display water
 *   You can easily create a water surface by just adding
 
     WaterNode *water = WaterNode::create();
     addChild(water);
 
 *   to your layer or node
 *   This node overrides the draw call, and it's where the magic happens
 */
class WaterNode : public cocos2d::Node
{
public:
    // creation
    static WaterNode *create()
    {
        auto ret = new WaterNode();
        ret->init();
        ret->autorelease();
        return ret;
    }
    ~WaterNode();
    
    // variables
    std::vector<WaterColumn*> columns;      // a vector of WaterColumn (nodes, with no autorelease) that represent slices of the water surface
    float scale;                            // the scale, used to determine the width of the surface and the force of the splash
    Vertex vertexArray[kColumnCount * 2];   // the array of Vertex
    Color colorArray[kColumnCount * 2];     // the array of Colors
    cocos2d::CustomCommand customCommand;   // we need a CustomCommand to properly set the zOrder of the Node
    
    // functions (read .cpp for comments)
    bool init() override;                   
    void initColors();
    void initColumns();
    void update();
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    void onCustomDraw(const cocos2d::Mat4 &transform);
    void splash(float index, float speed);
};