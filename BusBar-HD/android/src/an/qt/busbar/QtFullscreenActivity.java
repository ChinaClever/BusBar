//package an.qt.busbar;
//import android.content.Context;
//import android.content.Intent;
//import android.app.PendingIntent;
//import android.util.Log;
//import android.os.Bundle;
//import android.view.WindowManager;
 
//public class QtFullscreenActivity extends org.qtproject.qt5.android.bindings.QtActivity
//{
//    private final static String TAG = "QtFullscreen";
//    @Override
//    public void onCreate(Bundle savedInstanceState) {
//          super.onCreate(savedInstanceState);
//          getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
//    }
//}
package an.qt.busbar;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.util.Log;
import android.os.Bundle;
import android.view.View;
import android.view.MotionEvent;
import android.view.WindowManager;

public class QtFullscreenActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private final static String TAG = "QtFullscreen";
    @Override
    public void onCreate(Bundle savedInstanceState) {
          super.onCreate(savedInstanceState);
          getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                | View.SYSTEM_UI_FLAG_FULLSCREEN      // hide status bar
                | View.SYSTEM_UI_FLAG_IMMERSIVE);
    }

//    @Override
//    public boolean dispatchTouchEvent(MotionEvent ev) {
//        getWindow().getDecorView().setSystemUiVisibility(
//                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
//                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
//                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
//                | View.SYSTEM_UI_FLAG_FULLSCREEN      // hide status bar
//                | View.SYSTEM_UI_FLAG_IMMERSIVE);
//        return super.dispatchTouchEvent(ev);
//    }
}
