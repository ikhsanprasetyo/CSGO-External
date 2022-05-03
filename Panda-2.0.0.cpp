#include "../Panda-2.0.0/Memory.h"
using namespace std;

/*namespace Offsets
{
    constexpr uintptr_t localPlayer = 0xDB75DC;
    constexpr uintptr_t flags = 0x104;
    constexpr uintptr_t forceJump = 0x527D370;
    constexpr uintptr_t entityList = 0x4DD345C;
    constexpr uintptr_t glowObjectManager = 0x531C060;
    constexpr uintptr_t teamNum = 0xF4;
    constexpr uintptr_t glowIndex = 0x10488;
    constexpr uintptr_t bSpotted = 0x93D;
    //triggerbot
    constexpr uintptr_t ForceAttack = 0x3203928;
    constexpr uintptr_t m_iHealth = 0x100;
    constexpr uintptr_t m_iCrosshairId = 0x11838;
    //recoil control system
    constexpr uintptr_t dwClientState = 0x58CFC4;
    constexpr uintptr_t dwClientState_ViewAngles = 0x4D90;
    constexpr uintptr_t m_aimPunchAngle = 0x303C;
    constexpr uintptr_t m_iShotsFired = 0x103E0;
}*/
namespace Offsets
{
    constexpr uintptr_t dwLocalPlayer = 0xDB75DC;
    constexpr uintptr_t m_fFlags = 0x104;
    constexpr uintptr_t dwForceJump = 0x527D370;
    constexpr uintptr_t dwEntityList = 0x4DD345C;
    constexpr uintptr_t dwGlowObjectManager = 0x531C060;
    constexpr uintptr_t m_iTeamNum = 0xF4;
    constexpr uintptr_t m_iGlowIndex = 0x10488;
    constexpr uintptr_t m_bSpotted = 0x93D;
    //triggerbot
    constexpr uintptr_t dwForceAttack = 0x3203928;
    constexpr uintptr_t m_iHealth = 0x100;
    constexpr uintptr_t m_iCrosshairId = 0x11838;
    //recoil control system
    constexpr uintptr_t dwClientState = 0x58CFC4;
    constexpr uintptr_t dwClientState_ViewAngles = 0x4D90;
    constexpr uintptr_t m_aimPunchAngle = 0x303C;
    constexpr uintptr_t m_iShotsFired = 0x103E0;
};


__declspec(align(16)) struct Color
{
    constexpr Color(const float r, const float g, const float b, const float a = 1.f) noexcept :
        r(r), g(g), b(b), a(a) { }

    float r, g, b, a;
};

struct Vector2
{
    float x = { }, y = { };
};

Vector2 viewAngles;
Vector2 aimPunch;
Vector2 oldPunch = Vector2{ };
Vector2 newAngles;


DataAddress dataAddr;
Pattern sig;

int main()
{
    /* HINSTANCE hInstance = NULL;
    WNDCLASSEXA OverlayWnd;
    OverlayWnd.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 48, 48, 0)); //LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
    OverlayWnd.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 24, 24, 0)); //LoadIcon(NULL, IDI_APPLICATION); // basic window icon set*/
   
    std::string_view csgo = "csgo.exe";
    std::string_view clientName = "client.dll";
    std::string_view engineName = "engine.dll";

    //const wchar_t* ccsgo = L"csgo.exe";
    //const wchar_t* cclientName = L"client.dll";
    //const wchar_t* cengineName = L"engine.dll";

    const auto memory = Memory(csgo);
    std::cout << "dapat csgo!" << std::endl;

    const uintptr_t client = memory.GetModuleAddress(clientName);
    const uintptr_t engine = memory.GetModuleAddress(engineName);
    std::cout << "client.dll -> " << "0x" << std::hex << client << std::dec << std::endl;
    std::cout << "engine.dll -> " << "0x" << std::hex << engine << std::dec << std::endl;

    constexpr const auto color = Color{ 1.f, 0.f, 1.f };
    
    /*if (dataAddr.localPlayer == NULL && memory.processHandle != NULL)
    {
        dataAddr.localPlayer = PatternScanExModule(memory.processHandle, ccsgo, cclientName, sig.dwLocalPlayerPtr, sig.dwLocalPlayerMask);
        if (dataAddr.localPlayer < 0x1)
        {
            std::cout << "GAGAL SigScan! client.dll -> dwLocalPlayerPtr" << "0x" << std::hex << dataAddr.localPlayer << std::dec << std::endl;
        }
        else
        {
            std::cout << "client.dll -> dwLocalPlayerPtr" << "0x" << std::hex << dataAddr.localPlayer << std::dec << std::endl;
        }
    }*/
    
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        const uintptr_t localPlayer = memory.Read<std::uintptr_t>(client + Offsets::dwLocalPlayer);

        if (!localPlayer) continue;

        const uintptr_t localPlayerTeam = memory.Read<uintptr_t>(localPlayer + Offsets::m_iTeamNum);
        const uintptr_t localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + Offsets::m_fFlags);
        const uintptr_t localHealth = memory.Read<std::uintptr_t>(localPlayer + Offsets::m_iHealth);

        // bhop
        if (GetAsyncKeyState(VK_SPACE))
            (localPlayerFlags & (1 << 0)) ?
            memory.Write<std::uintptr_t>(client + Offsets::dwForceJump, 6) :
            memory.Write<std::uintptr_t>(client + Offsets::dwForceJump, 4);

        // glow
        const uintptr_t glowObjectManager = memory.Read<std::uintptr_t>(client + Offsets::dwGlowObjectManager);

        for (int i = 1; i <= 32; ++i)
        {
            const uintptr_t entity = memory.Read<std::uintptr_t>(client + Offsets::dwEntityList + i * 0x10);

            if (!entity)
                continue;

            // dont glow if they are on our team
            if (memory.Read<std::uintptr_t>(entity + Offsets::m_iTeamNum) == localPlayerTeam)
                continue;

            //const int32_t glowIndex = memory.Read<std::int32_t>(entity + Offsets::glowIndex);

            // do glow by writing each variable
            //memory.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, 1.f);
            //memory.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, 0.f);
            //memory.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, 0.f);
            //memory.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, 1.f);

            //memory.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
            //memory.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);

            // preferred
            // use a color struct to make 1 WPM call
            //memory.Write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color);

            //memory.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
            //memory.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, false);

            memory.Write<bool>(entity + Offsets::m_bSpotted, true);

            
        }
        //triggerbot
        if (GetAsyncKeyState(VK_MENU))
        {
            if (!localHealth) continue;

            const int32_t crosshairId = memory.Read<std::int32_t>(localPlayer + Offsets::m_iCrosshairId);

            if (!crosshairId || crosshairId > 64) continue;

            const uintptr_t player = memory.Read<std::uintptr_t>(client + Offsets::dwEntityList + (crosshairId - 1) * 0x10);

            //skip if player is dead
            if (!memory.Read<std::int32_t>(player + Offsets::m_iHealth)) continue;

            //skip if player is on our team
            if (memory.Read<std::int32_t>(player + Offsets::m_iTeamNum) == memory.Read<std::int32_t>(localPlayer + Offsets::m_iTeamNum)) continue;

            memory.Write<std::uintptr_t>(client + Offsets::dwForceAttack, 6);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            memory.Write<std::uintptr_t>(client + Offsets::dwForceAttack, 4);
        }
        //recoil control system
        const std::int32_t shotsFired = memory.Read<std::int32_t>(localPlayer + Offsets::m_iShotsFired);

        if (shotsFired)
        {
            const std::uintptr_t clientState = memory.Read<std::uintptr_t>(engine + Offsets::dwClientState);
            viewAngles = memory.Read<Vector2>(clientState + Offsets::dwClientState_ViewAngles);

            aimPunch = memory.Read<Vector2>(localPlayer + Offsets::m_aimPunchAngle);

            
            newAngles.x = viewAngles.x + oldPunch.x - aimPunch.x * 2.0f;
            newAngles.y = viewAngles.y + oldPunch.y - aimPunch.y * 2.0f;
            

            if (newAngles.x > 89.0f)
            {
                newAngles.x = 89.0f;
            }
            if (newAngles.x < -89.0f)
            {
                newAngles.x = -89.0f;
            }
            while (newAngles.y > 180.0f)
            {
                newAngles.y -= 360.f;
            }
            while (newAngles.y < -180.0f)
            {
                newAngles.y += 360.f;
            }

            memory.Write<Vector2>(clientState + Offsets::dwClientState_ViewAngles, newAngles);

            oldPunch.x = aimPunch.x * 2.0f;
            oldPunch.y = aimPunch.y * 2.0f;
        }
        else
        {
            oldPunch.x = oldPunch.y = 0.0f;
        }
        

    }
    
}


