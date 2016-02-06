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
#include "WaterNode.hpp"

using namespace cocos2d;

bool WaterNode::init()
{
    // super init
    Node::init();
    
    // setup the node information and the columsn (WaterColumn)
    scale = Director::getInstance()->getVisibleSize().width/(kColumnCount - 1);
    this->initColors();
    this->initColumns();
    
    // this node will have the SHADER_NAME_POSITION_COLOR shader
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    
    return true;
}

void WaterNode::initColors()
{
    // init the vertex colors, with two different colors
    for (int i = 1; i < kColumnCount * 2; i += 2)
    {
        colorArray[i - 1].color = topColor;
        colorArray[i].color = bottomColor;
    }
}

void WaterNode::initColumns()
{
    // init the WaterColumns and push them into an array so we can access them later on
    columns = {};
    for (int i = 0; i < kColumnCount; i++)
    {
        WaterColumn *column = WaterColumn::create();
        column->initWithTargetHeight(kWaterHeight, kWaterHeight, 0);
        columns.push_back(column);
    }
}

WaterNode::~WaterNode()
{
    // clean up the resources we created (remember, WaterColumn is not autorelease)
    columns.clear();
}

void WaterNode::update()
{
    // the update function updates the logical state of the columns based on the various parameters so the draw will draw precisly what we want
    for (int i = 0; i < kColumnCount; i++)
    {
        WaterColumn *waterColumn = columns[i];
        waterColumn->update(kDampening, kTension);
    }

    float leftDeltas[kColumnCount];
    float rightDeltas[kColumnCount];

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < kColumnCount; i++)
        {
            if (i > 0)
            {
                leftDeltas[i] = kSpread * (columns[i]->height - columns[i-1]->height);
                ((WaterColumn *) columns[i - 1])->speed += leftDeltas[i];
            }
            if (i < kColumnCount - 1)
            {
                rightDeltas[i] = kSpread * (columns[i]->height - columns[i+1]->height);
                ((WaterColumn *) columns[i + 1])->speed += rightDeltas[i];
            }
        }

        for (int i = 0; i < kColumnCount; i++)
        {
            if (i > 0)
            {
                ((WaterColumn *)columns[i - 1])->height += leftDeltas[i];
            }
            if (i < kColumnCount - 1)
            {
                ((WaterColumn *)columns[i + 1])->height += rightDeltas[i];
            }
        }
    }
}

void WaterNode::onCustomDraw(const Mat4 &transform)
{
    // update the matrix
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    // update the parameters of vertex and colors
    update();
    for (int i = 0; i < kColumnCount; i++)
    {
        uint16_t x = (i * scale);
        uint16_t y = columns[i]->height;
        
        vertexArray[2 * i] = (struct Vertex) {x, y};
        vertexArray[2 * i + 1] = (struct Vertex) {x, 0};
    }
    
    // now finally call the low-level opengGL calls to render the GL_TRIANGLE_STRIP in a simple way
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    this->getGLProgram()->use();
    this->getGLProgram()->setUniformsForBuiltins();
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, vertexArray);
    glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colorArray);
    glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_COLOR);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, kColumnCount * 2);
}

void WaterNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    /*
     *  We override the basic draw to use our own
     *  Here we actually just create a CustomCommand (custom openGL command) to 'force' the zGlobalOrder we want for this node
     *  Without this approach, we would see our node drawn behind EVERYTHING else
     *  With this code we can specifiy the zGlobalOrder we want
     *  More information on the subject here: http://stackoverflow.com/questions/22549233/cocos2d-x-3-0-draw-vs-ondraw/22724319#22724319
     */
    customCommand.init(_globalZOrder);
    customCommand.func = CC_CALLBACK_0(WaterNode::onCustomDraw, this, transform);
    renderer->addCommand(&customCommand);
}

void WaterNode::splash(float x, float speed)
{
    // called when we want to simulate a splash in the water with speed value (usually negative, but works with positive too)
    int index = x/scale;
    if (index > 0 && index < kColumnCount)
    {
        WaterColumn *waterColumn = columns[index];
        waterColumn->speed = speed;
    }
}