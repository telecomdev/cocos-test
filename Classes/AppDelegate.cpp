#include "AppDelegate.h"
#include "GLGray.h"
#include "GLTriangle.h"
#include "GLTriangleVAO.h"
#include "GLQuadVAO.h"
#include "GLCubeVAO.h"
#include "GLTextureVAO.h"
#include "tortoise/HelloWorldScene.h"
#include "GLTextureTest.h"
#include "UVSpriteTest.h"
#include "ClippingTest.h"
#include "TableTest.h"
#include "MotionBlurTest.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }


	std::vector<std::string> searchPath;
	searchPath.push_back("3D");
	// set searching path
	FileUtils::getInstance()->setSearchPaths(searchPath);




    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    // create a scene. it's an autorelease object

	// gray test
    //auto scene = GLGray::createScene();
	// openGL draw triangle
	//auto scene = GLTriangle::createScene();
	// openGL draw triangle via VAO
	//auto scene = GLTriangleVAO::createScene();
	// openGL draw Quad via VAO
	//auto scene = GLQuadVAO::createScene();
	// openGL draw Cube via VAO
	//auto scene = GLCubeVAO::createScene();
	// openGL draw Texture via VAO
	//auto scene = GLTextureVAO::createScene();
	
	//auto scene = GLTextureTest::createScene();
	//auto scene = UVSpriteTest::createScene();
	//auto scene = TableTest::createScene();
	auto scene = MotionBlurTest::createScene();
	
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
