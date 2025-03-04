#ifdef DO_NOT_COMPILE

struct InputMessage : public Printable
{
    int x;
    int y;

    virtual size_t printTo(Print& p) const override
    {
        return p.printf("-InputMessage- {x: %i, y: %i}", x, y);
    }
};

REGISTER_MESSAGE_TYPE(InputMessage, 3);

BrainApp::BrainApp(const AppConfig& config)
    : state(ConnectionState::BROADCASTING),
    isComplete(false),
    m_Timeout()
{}

void BrainApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    AddComponent<WifiComponent>();
    SetupComponents();

    WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("connection", [this](ConnectionRequest msg) {
        if (state != ConnectionState::BROADCASTING) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.from;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("handshake", [this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::CONNECTED;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<InputMessage>("input", [this](InputMessage msg) {
        LogInfo("Received: ", msg);
        isComplete = state == ConnectionState::CONNECTED;
    });
    WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);

    #ifdef IS_SIMULATOR
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest>("connection", [this](ConnectionRequest msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        ConnectionRequest connectMsg(otherMac);
        WifiModule::GetInstance().SimulateSend(msg);
    });
    WifiModule::GetInstance().RegisterSimulateSendCallback<Handshake>("handshake", [this](Handshake msg) {
        Handshake handshake(MacAddress({ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }));
        WifiModule::GetInstance().SimulateSend(handshake);
        InputMessage inoutMsg;
        inoutMsg.x = 10;
        inoutMsg.y = 20;
        WifiModule::GetInstance().SimulateSend(inoutMsg);
        delay(1000);
    });
    #endif
}

void BrainApp::Update()
{
    UpdateComponents();

    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::BROADCASTING)
    {
        LogInfo("Sending connection request...");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(BROADCAST_ADDRESS, request);
        delay(BRAIN_BROADCAST_DELAY);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        //add a timer here to return to BROADCASTING after a while + add broadcast peer again
        WifiModule::GetInstance().RemovePeer(BROADCAST_ADDRESS);
        WifiModule::GetInstance().AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        //remove timer
    }
}

ControllerApp::ControllerApp(const AppConfig& config)
    : state(ConnectionState::WAITING_FOR_CONNECTION), isComplete(false)
{}

void ControllerApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    AddComponent<WifiComponent>();
    SetupComponents();

    WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("connection", [this](ConnectionRequest msg) {
        if (state != ConnectionState::WAITING_FOR_CONNECTION) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("input", [this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = SENDING_HANDSHAKE;
    });

    #ifdef IS_SIMULATOR
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest>("connection", [this](ConnectionRequest msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        Handshake hand(otherMac);
        WifiModule::GetInstance().SimulateSend(hand);
    });
    #endif
}

void ControllerApp::Update()
{
    UpdateComponents();

    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::WAITING_FOR_CONNECTION)
    {
        LogInfo("Waiting for connection request");
        delay(CONTROLLER_CONNECTION_DELAY);
        #ifdef IS_SIMULATOR
                //Simulate a connection from brain
        MacAddress receiverMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        ConnectionRequest msg(receiverMac);
        WifiModule::GetInstance().SimulateSend(msg);
        #endif
    }

    if (state == ConnectionState::SENDING_REQUEST)
    {
        WifiModule::GetInstance().AddPeer(brainMac);
        LogInfo("Sending connection request.");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(request);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
        return;
    }
    if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = ConnectionState::CONNECTED;
        return;
    }
    if (state == ConnectionState::CONNECTED && !isComplete)
    {
        LogInfo("Sending input message");
        InputMessage msg;
        msg.x = 10;
        msg.y = 20;
        WifiModule::GetInstance().Send(msg);
        isComplete = true;
    }
}



#endif