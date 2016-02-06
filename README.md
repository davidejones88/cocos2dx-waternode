# cocos2dx-WaterNode
A ready-to-go cocos2dx (3.x) Node to simulate 2d water effect with splash capabilities

# Example

# How to use
1. Simply import "WaterNode.hpp" where you want to use the WaterNode and create it using

```
WaterNode water = WaterNode::create();
addChild(water);
```

2. In order to make the water move you need to call the ```splash``` method, for example in the cocos2dx ```onTouchEnded``` method

```
water->splash(touch->getLocation().x, -300);
```

3. You can customize the behaviour of the water surface tweaking the following values in WaterNode.hpp

```
static const int kColumnCount = 80;
static const int kWaterHeight = 500;
static const float kTension = 0.025;
static const float kDampening = 0.025;
static const float kSpread = 0.25;
static cocos2d::Color4B topColor = cocos2d::Color4B(0, 50, 200, 255);
static cocos2d::Color4B bottomColor = cocos2d::Color4B(0, 50, 100, 100);
```

# Credits
This code was ported from https://github.com/mrMakaronka/ios-2d-water-simulation