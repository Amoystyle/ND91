package com.nd.assistance.ui;

import java.util.Locale;

import com.nd.assistance.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.TextView;

public class HelpActivity extends Activity {
	private WebView mWebView;

	
	private int mCount =0;
	private long mLastTime=0;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.help);
		mWebView = (WebView) findViewById(R.id.webViewHelp);
		mWebView.setWebViewClient(new WebViewClient() {

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				view.loadUrl(url);
				return true;
			}
			
		});
		WebSettings ws = mWebView.getSettings();
		ws.setBuiltInZoomControls(true);
		Button imgButton = (Button) findViewById(R.id.buttonBack);
		imgButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				HelpActivity.this.finish();
			}
		});
	    String language = Locale.getDefault().getCountry();
		String url = null;	
						
		if (language.equals("TW")) {
			url = "file:///android_asset/TW/faq_common.shtml";
		}
		else {
			url = "file:///android_asset/faq_common.shtml";
		}
		mWebView.loadUrl(url);
		
		TextView textViewTitle = (TextView)findViewById(R.id.textViewTitle);
		mLastTime = System.currentTimeMillis();
		textViewTitle.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if(System.currentTimeMillis() -mLastTime>1000){
					mCount =0;
				}else{
					mCount++;
					if(mCount==7){
						HelpActivity.this.startActivity(new Intent
								(HelpActivity.this, TestActivity.class));
						mCount =0;
					}
				}
				mLastTime = System.currentTimeMillis();
				
			}
		});
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (KeyEvent.KEYCODE_BACK == keyCode && mWebView.canGoBack()) {
			mWebView.goBack();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}
	
}
