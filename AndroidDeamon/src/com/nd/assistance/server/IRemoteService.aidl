package com.nd.assistance.server;

import com.nd.assistance.server.IServiceCallBack;

interface IRemoteService {
    
    void registerCallback(IServiceCallBack cb);
    
    String getConnectionInfos();
    
    void setWifiOff();
    
    boolean isWifiConnecting();
    
    void sendMessage(int action);
    
    boolean isConnecting();
    
    int getMediaStatus();
    
    void initLogCenter();
    
    
}
