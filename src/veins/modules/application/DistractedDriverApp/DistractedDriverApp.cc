#include "veins/modules/application/DistractedDriverApp/DistractedDriverApp.h"
#include <cstdio>

using namespace veins;

Define_Module(DistractedDriverApp);

void DistractedDriverApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        isDistracted = false;
        lastSent = simTime() + uniform(0, 1);
        messageInterval = 3.0;

        if (uniform(0, 1) < 0.50) {
            isDistracted = true;
            findHost()->getDisplayString().setTagArg("i", 1, "red");
            traciVehicle->setColor(TraCIColor(255, 0, 0, 255));
            EV << "Arac " << myId << " DIKKATSIZ (Kirmizi) olarak isaretlendi." << endl;
        }
    }
}

void DistractedDriverApp::handlePositionUpdate(cObject *obj) {
    DemoBaseApplLayer::handlePositionUpdate(obj);

    if (isDistracted && (simTime() - lastSent >= messageInterval)) {

        TraCIDemo11pMessage *wsm = new TraCIDemo11pMessage();
        populateWSM(wsm);

        char buf[100];
        snprintf(buf, sizeof(buf), "Tehlike %ld %d %d", (long)myId, (int)curPosition.x, (int)curPosition.y);
        wsm->setDemoData(buf);

        sendDown(wsm);
        lastSent = simTime();

        findHost()->getDisplayString().setTagArg("r", 0, "100");
    }
}

// DUZELTME: Burasi artik genel cerceve (BaseFrame) aliyor
void DistractedDriverApp::onWSM(BaseFrame1609_4 *frame) {

    // 1. Gelen mesaji dönüştür
    TraCIDemo11pMessage *wsm = dynamic_cast<TraCIDemo11pMessage*>(frame);
    if (!wsm) return;

    // 2. Kendi mesajimsa ilgilenme
    if (wsm->getSenderAddress() == myId) return;

    // 3. Mesajin icindeki koordinatlari oku
    long senderId;
    int senderX, senderY;
    const char* data = wsm->getDemoData();

    // Veriyi ayikla (Format: "Tehlike ID X Y")
    if (sscanf(data, "Tehlike %ld %d %d", &senderId, &senderX, &senderY) == 3) {

        // Mesafeyi hesapla
        Coord senderPos(senderX, senderY);
        double distance = curPosition.distance(senderPos);

        EV << "Mesaj alindi. Gonderen: " << senderId << " Mesafe: " << distance << "m" << endl;

        // --- ALGORITMANIN KALBI BURASI ---
        // Sadece 50 metreden daha yakinsam tepki ver
        if (distance < 50.0) {

            if (!isDistracted) {
                EV << "!!! KRITIK YAKINLIK (" << distance << "m) - FREN YAPILIYOR !!!" << endl;

                // Görsel Uyarı: SARI
                traciVehicle->setColor(TraCIColor(255, 255, 0, 255));
                findHost()->getDisplayString().setTagArg("i", 1, "yellow");

                // Fiziksel Tepki: Sert Fren (Hizi 5 saniyede 2 m/s'ye düşür)
                traciVehicle->slowDown(2.0, 5000);
            }
        } else {
            // Mesafe uzaksa tepki verme (Loga yaz sadece)
            EV << "Tehlike uzakta (" << distance << "m). Tepki verilmedi." << endl;
        }
    }
}
