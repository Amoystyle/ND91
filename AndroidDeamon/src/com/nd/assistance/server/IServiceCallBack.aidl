package com.nd.assistance.server;

interface IServiceCallBack {
    
    void connectionChanged(String connInfos);
    void pcMediaStatus(int status);
    
    
}