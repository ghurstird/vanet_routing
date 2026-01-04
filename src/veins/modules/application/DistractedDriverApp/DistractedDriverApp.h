#ifndef DISTRACTEDDRIVERAPP_H_
#define DISTRACTEDDRIVERAPP_H_

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

using namespace veins;

class DistractedDriverApp : public DemoBaseApplLayer {
public:
    virtual void initialize(int stage) override;

protected:
    virtual void handlePositionUpdate(cObject *obj) override;
    virtual void onWSM(BaseFrame1609_4 *frame) override;

private:
    bool isDistracted;
    simtime_t lastSent;
    simtime_t messageInterval;
};

#endif /* DISTRACTEDDRIVERAPP_H_ */
