package org.car2x.veins.modules.application.DistractedDriverApp;

// Import ederken de tam yolu kullanmamiz lazim
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple DistractedDriverApp extends DemoBaseApplLayer
{
    parameters:
        @class(DistractedDriverApp);
        headerLength = default(80 bit);
        sendBeacons = default(false); 
}