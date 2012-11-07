#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "cinder/params/Params.h"
#include "cinder/CinderMath.h"
#include "OscListener.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ButterflyApp : public AppBasic {
public:
    ButterflyApp();
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void flyButton();
    void prepareSettings( Settings  *settings );    
    
    float                   mSpeed, mDefaultSpeed;
    params::InterfaceGl     mParams;
    osc::Listener           listener;
    
    //shaderParams
    Color                   mTopWingColor;
    Color                   mStripeColor1;
    Color                   mStripeColor2;
    int                     mStripeMode;  //0, 1, 2
    float                   mStripeT;
    bool                    mFly;
    float                   mTailShape;
    float                   mTopFringeFactor, mBottomFringeFactor;
    float                   mTopFringeEffect, mBottomFringeEffect;
    
private:
    void drawRightWing();
    void drawLeftWing();
    
    gl::GlslProg    mShader, mShaderFly, mButterfly;
    gl::Fbo         mFbo;
	static const int	FBO_WIDTH = 500, FBO_HEIGHT = 500;    
    float           mTime;
    
    Vec2f           mStripeTimer;
    float           mStripeDuration;
    float           mStripeSpeed;
    float           mStripeTime;
    
};


ButterflyApp::ButterflyApp() : mTime( 0.0 ), mDefaultSpeed( .001 ) 
{
    mSpeed = mDefaultSpeed;    
    mStripeColor1 = Color(.5,0.0,0.0);
    mStripeColor2 = Color(0.36,0.68,0.0);
    mTopWingColor = Color(0.0,0.0,0.0);
    mStripeMode = 0;
    mStripeT = 1.0;
    
    mStripeTimer = Vec2f(1.0f,40.0f);
    mStripeDuration = 20.0f;
    mStripeSpeed = 0.0f;
    mStripeTime = 0.0;

    mTailShape = 7.0;   
    mFly = false;
    
    mTopFringeFactor = 24.0f;
    mBottomFringeFactor = 48.0f;
}

void ButterflyApp::setup()
{
    //Load Shaders
    try 
    {
        mShader = gl::GlslProg( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_BUTTERFLY_FRAG ) );
        mShaderFly = gl::GlslProg( loadResource( RES_FLAPPING_VERT ), loadResource( RES_PASSTHRU_FRAG ) );
        mButterfly = gl::GlslProg( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_BUTTERFLY_DECORATED_FS ) );        
    }
    catch( gl::GlslProgCompileExc &exc )
    { 
        cout << "Shader compile error: " << endl;
        cout << exc.what();
    }
    
    //Create FBO
    gl::Fbo::Format format;
	mFbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
    
    //Setup the Params
    mParams = params::InterfaceGl( "App parameters", Vec2i( 200, 400 ) );
    mParams.addText( "text", "label=`Butterfly Builder.`" );
    mParams.addButton( "Fly?", std::bind( &ButterflyApp::flyButton, this ));
    mParams.addParam( "Wing Color", &mTopWingColor );
    mParams.addParam( "Stripe Color 1", &mStripeColor1 );
    mParams.addParam( "Stripe Color 2", &mStripeColor2 );    
    mParams.addParam( "Striping Type", &mStripeMode, "min=0 max=2 step=1 keyIncr=M keyDecr=m" );
    mParams.addParam( "Stripe Speed", &mStripeSpeed, "min=0.0 max=0.5 step=0.005 keyIncr=S keyDecr=s" );
    mParams.addParam( "Tail Hook", &mTailShape, "min=1.0 max=30 step=0.25 keyIncr=L keyDecr=l" );
    mParams.addParam( "Top Feathers", &mTopFringeFactor, "min=1.0 max=48.0 step=0.5" );
    mParams.addParam( "Bottom Feathers", &mBottomFringeFactor, "min=1.0 max=48.0 step=0.5" );    
    mParams.addParam( "Top Feather Style", &mTopFringeEffect, "min=0.0 max=0.1 step=0.005" );
    mParams.addParam( "Bottom Feather Style", &mBottomFringeEffect, "min=0.0 max=0.08 step=0.005" );    
}

void ButterflyApp::mouseDown( MouseEvent event )
{
}

void ButterflyApp::update()
{
    if( mFly ) 
    {
        mSpeed = mDefaultSpeed;
        mTime += mSpeed;
    }
    else 
    {
        mSpeed = 0.0f;
    }
    
    mStripeTime += mStripeSpeed;    
    float norm_t = mStripeTime  / mStripeDuration;
    
    norm_t = cinder::math<float>::clamp(norm_t, 0.0f, 1.0f);
    if( norm_t == 1.0 )
    {
        mStripeTime = 0.0;
    }
    
    mStripeT = mStripeTimer.x * (1.0f - norm_t) + mStripeTimer.y * norm_t;
    
}

void ButterflyApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 1.0f, 1.0f, 1.0f ) ); 
    
    //    cout << "{0} get View port = " << gl::getViewport() << endl;    
    
    mFbo.bindFramebuffer();
    gl::setViewport( mFbo.getBounds() );
    gl::setMatricesWindow( mFbo.getSize(), false );
    //    cout << "{1} get View port = " << gl::getViewport() << endl;        
    
    gl::clear();
    gl::color( Color::white() );
    mButterfly.bind();
    mButterfly.uniform( "resolution", Vec2f( mFbo.getWidth(), mFbo.getHeight() ) );
    mButterfly.uniform( "topWingC", mTopWingColor); //Vec3f(mTopWingColor.r,mTopWingColor.g,mTopWingColor.b) );
    mButterfly.uniform( "stripeC1", mStripeColor1 );    
    mButterfly.uniform( "stripeC2", mStripeColor2 );    
    mButterfly.uniform( "stripeT", mStripeT ); 
    mButterfly.uniform( "stripeFn", mStripeMode );
    mButterfly.uniform( "tailShape", mTailShape );
    mButterfly.uniform( "topFringeFactor", mTopFringeFactor );
    mButterfly.uniform( "bottomFringeFactor", mBottomFringeFactor );    
    mButterfly.uniform( "topFringeEffect", mTopFringeEffect );
    mButterfly.uniform( "bottomFringeEffect", mBottomFringeEffect );      
    gl::drawSolidRect( mFbo.getBounds() );  
    mButterfly.unbind();
    mFbo.unbindFramebuffer();
    
    
    glLoadIdentity();
    gl::enable( GL_TEXTURE_2D );    
    
    Area v1 = Area( Rectf( 100, 480, 500, 80 ) );
    gl::setViewport( v1 ); 
    //    gl::setViewport( mFbo.getBounds() );    
    gl::enable( GL_TEXTURE_2D );
    mFbo.bindTexture();    
    
    mShaderFly.bind();
    mShaderFly.uniform( "tex", 0 );

    mShaderFly.uniform("t", mTime);    
    
    drawRightWing();    
    mShaderFly.unbind();    
    
    mShaderFly.bind();
    mShaderFly.uniform( "tex", 0 );
    mShaderFly.uniform("t", mTime + mSpeed * 0.5f);    
    
    drawLeftWing();
    mShaderFly.unbind();    
    
    mFbo.unbindTexture();    
    
    
    gl::disable( GL_TEXTURE_2D );
    
    params::InterfaceGl::draw();
}

void ButterflyApp::drawRightWing() {
	gl::begin( GL_TRIANGLES );
    
	// Define quad vertices
	Area bounds = mFbo.getBounds();
    float half_width = bounds.getWidth() * 0.5;
	Vec2f vert0( (float)bounds.x1 + half_width, (float)bounds.y1 );
	Vec2f vert1( (float)bounds.x2, (float)bounds.y1 );
	Vec2f vert2( (float)bounds.x1 + half_width, (float)bounds.y2 );
	Vec2f vert3( (float)bounds.x2, (float)bounds.y2 );
    
	// Define quad texture coordinates
	Vec2f uv0( 0.5f, 0.0f );
	Vec2f uv1( 1.0f, 0.0f );
	Vec2f uv2( 0.5f, 1.0f );
	Vec2f uv3( 1.0f, 1.0f );
    
	// Draw quad (two triangles)
	gl::texCoord( uv0 );
	gl::vertex( vert0 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv1 );
	gl::vertex( vert1 );
    
	gl::texCoord( uv1 );
	gl::vertex( vert1 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv3 );
	gl::vertex( vert3 );
    
	// End drawing
	gl::end();       
    
}

void ButterflyApp::drawLeftWing() {
	gl::begin( GL_TRIANGLES );
    
	// Define quad vertices
	Area bounds = mFbo.getBounds();
    float half_width = bounds.getWidth() * 0.5;
	Vec2f vert0( (float)bounds.x1, (float)bounds.y1 );
	Vec2f vert1( (float)bounds.x1 + half_width, (float)bounds.y1 );
	Vec2f vert2( (float)bounds.x1, (float)bounds.y2 );
	Vec2f vert3( (float)bounds.x1 + half_width, (float)bounds.y2 );
    
	// Define quad texture coordinates
	Vec2f uv0( 0.0f, 0.0f );
	Vec2f uv1( 0.5f, 0.0f );
	Vec2f uv2( 0.0f, 1.0f );
	Vec2f uv3( 0.5f, 1.0f );
    
	// Draw quad (two triangles)
	gl::texCoord( uv0 );
	gl::vertex( vert0 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv1 );
	gl::vertex( vert1 );
    
	gl::texCoord( uv1 );
	gl::vertex( vert1 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv3 );
	gl::vertex( vert3 );
    
	// End drawing
	gl::end();     
    
}

void ButterflyApp::prepareSettings( Settings  *settings ) {
    settings->setFrameRate( 60.0f );
    settings->setWindowSize( 640, 480 );
}

void ButterflyApp::flyButton()
{
    mFly = !mFly;
}


CINDER_APP_BASIC( ButterflyApp, RendererGl )
