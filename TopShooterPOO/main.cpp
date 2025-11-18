#include "raylib.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

// Enums para estados do jogo
enum GameState {
    MENU,
    PLAYING,
    SHOP,
    STATUS,
    SETTINGS,
    PAUSE_MENU,
    INVENTORY,
    CHEAT_MENU
};
struct Player {
    Vector2 position;
    float speed;
    float radius;
    Color color;
    int health;
    int maxHealth;
    Texture2D texture;
    float rotation;
    int ammo;
    int maxAmmo;
    int level;
    int xp;
    int xpToNextLevel;
    int coins;
};

struct Enemy {
    Vector2 position;
    float speed;
    float radius;
    Color color;
    bool active;
    int health;
    int maxHealth;
    bool isBoss;
    Texture2D texture;
    float rotation;
    int xpDrop;
    int coinDrop;
};

struct Projectile {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
    int damage;
    Texture2D texture;
    float rotation;
};

enum PickupType {
    PICKUP_HEALTH,
    PICKUP_LOOT
};

struct Pickup {
    Vector2 position;
    float radius;
    bool active;
    PickupType type;
    Texture2D texture;
};

enum AmmoRarity {
    AMMO_COMMON,
    AMMO_UNCOMMON,
    AMMO_RARE,
    AMMO_LEGENDARY
};

struct AmmoItem {
    AmmoRarity rarity;
    float damageMultiplier;
    Texture2D texture;
    const char* name;
    Color color;
};

enum ChestState {
    CHEST_CLOSED,
    CHEST_OPENING,
    CHEST_OPENED
};

struct Chest {
    Vector2 position;
    float radius;
    bool active;
    ChestState state;
    Texture2D texture;
    int currentFrame;
    float animTimer;
    AmmoRarity itemDrop;
    bool itemRevealed;
    float revealTimer;
};

bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2) {
    float dx = center1.x - center2.x;
    float dy = center1.y - center2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (radius1 + radius2);
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Top-Down Game - POO2");
    SetTargetFPS(60);
    InitAudioDevice();
    
    GameState gameState = MENU;
    int selectedMenuOption = 0;
    int selectedPauseOption = 0;
    int selectedShopOption = 0; // 0=Dano, 1=Recarga, 2=Vel.Bala, 3=Munição, 4=Vida
    int shopScrollOffset = 0;
    int selectedCheatOption = 0;
    Texture2D playerTexture = LoadTexture("assets/entities/player/Survivor 1/survivor1_gun.png");
    Texture2D playerReloadTexture = LoadTexture("assets/entities/player/Survivor 1/survivor1_reload.png");
    Texture2D enemyTexture = LoadTexture("assets/entities/enemies/Zombie 1/zoimbie1_hold.png");
    Texture2D bulletTexture = LoadTexture("assets/entities/guns/weapon_gun.png");
    Texture2D groundTexture = LoadTexture("assets/img/ground/groundbase02.jpg");
    Texture2D groundBossTexture = LoadTexture("assets/img/ground/groundbaseboss.jpg");
    Texture2D healthPickupTexture = LoadTexture("assets/sptboost/lifebonus.png");
    Texture2D lootPickupTexture = LoadTexture("assets/sptboost/lootbonus.png");
    Texture2D waterBulletTexture = LoadTexture("assets/entities/guns/waterbullet.png");
    Texture2D fireBulletTexture = LoadTexture("assets/entities/guns/firebullet.png");
    Texture2D acidBulletTexture = LoadTexture("assets/entities/guns/acidbullet.png");
    Texture2D magicBulletTexture = LoadTexture("assets/entities/guns/magicbullet.png");
    
    Texture2D chestTextures[6];
    chestTextures[0] = LoadTexture("assets/entities/lootchest/metalchest1.png");
    chestTextures[1] = LoadTexture("assets/entities/lootchest/metalchest2.png");
    chestTextures[2] = LoadTexture("assets/entities/lootchest/metalchest3.png");
    chestTextures[3] = LoadTexture("assets/entities/lootchest/metalchest4.png");
    chestTextures[4] = LoadTexture("assets/entities/lootchest/metalchest5.png");
    chestTextures[5] = LoadTexture("assets/entities/lootchest/metalchest6.png");
    
    Texture2D bgMenu = LoadTexture("assets/img/background/bgmenu.jpg");
    Texture2D bgLightBrick = LoadTexture("assets/img/background/bglightbrick.jpg");
    Texture2D bgDarkBrick = LoadTexture("assets/img/background/bgdarkbrick.jpg");
    
    Texture2D playerLifeFull = LoadTexture("assets/UI/playerfulllifeUI.png");
    Texture2D playerLifeAlmostFull = LoadTexture("assets/UI/playeralmostfulllifeUI.png");
    Texture2D playerLifeHalf = LoadTexture("assets/UI/playerhalflifeUI.png");
    Texture2D playerLifeEnd = LoadTexture("assets/UI/playerendlifeUI.png");
    Texture2D playerLifeEmpty = LoadTexture("assets/UI/playeremptylifeUI.png");
    
    Texture2D enemyLifeFull = LoadTexture("assets/UI/enemyfulllifeUI.png");
    Texture2D enemyLifeAlmostFull = LoadTexture("assets/UI/enemyalmostfulllifeUI.png");
    Texture2D enemyLifeHalf = LoadTexture("assets/UI/enemyhalflifeUI.png");
    Texture2D enemyLifeEnd = LoadTexture("assets/UI/enemyendlifeUI.png");
    Texture2D enemyLifeEmpty = LoadTexture("assets/UI/enemyemptyUI.png");
    
    Texture2D bulletsFull = LoadTexture("assets/UI/bulletsfullUI.png");
    Texture2D bulletsAlmostFull = LoadTexture("assets/UI/bulletsalmostfullUI.png");
    Texture2D bulletsHalf = LoadTexture("assets/UI/halfbulletsUI.png");
    Texture2D bulletsAlmostEnd = LoadTexture("assets/UI/bulletsalmostendUI.png");
    Texture2D bulletsEnd = LoadTexture("assets/UI/bulletsendUI.png");
    Texture2D bulletsEmpty = LoadTexture("assets/UI/bulletsemptyUI.png");
    
    Texture2D configLabel = LoadTexture("assets/UI/configlabel.png");
    Texture2D musicLabel = LoadTexture("assets/UI/musiclabel.png");
    Texture2D soundLabel = LoadTexture("assets/UI/soundlabel.png");
    Texture2D volumeLabel = LoadTexture("assets/UI/volumelabel.png");
    Texture2D muteLabel = LoadTexture("assets/UI/mutelabel.png");
    Texture2D yesOption = LoadTexture("assets/UI/yesoptions.png");
    Texture2D noOption = LoadTexture("assets/UI/nooptions.png");
    Texture2D dotColor = LoadTexture("assets/UI/dotcolor.png");
    Texture2D dotEmpty = LoadTexture("assets/UI/dotempty.png");
    Texture2D frameLabel = LoadTexture("assets/UI/framelabel01.png");
    Texture2D whiteSquareUI = LoadTexture("assets/UI/whitesquareUI.png");
    Texture2D brownSquaresUI = LoadTexture("assets/UI/brownsquaresUI.png");
    Texture2D minusButton = LoadTexture("assets/UI/Minus.png");
    Texture2D plusButton = LoadTexture("assets/UI/Plus.png");
    
    int musicVolumeLevel = 2;
    int sfxVolumeLevel = 2;
    const float maxMusicVolumeMenu = 0.15f;
    const float maxMusicVolumeLevel = 0.15f;
    const float maxMusicVolumeBoss = 0.20f;
    const float maxSFXVolume = 0.4f;
    bool musicMuted = false;
    bool sfxMuted = false;
    
    Music menuMusic = LoadMusicStream("assets/sounds/music/menusound.mp3");
    Music levelMusic = LoadMusicStream("assets/sounds/music/levelmusic.mp3");
    Music bossMusic = LoadMusicStream("assets/sounds/music/bossmusic.mp3");
    
    SetMusicVolume(menuMusic, maxMusicVolumeMenu * (musicVolumeLevel / 6.0f));
    SetMusicVolume(levelMusic, maxMusicVolumeLevel * (musicVolumeLevel / 6.0f));
    SetMusicVolume(bossMusic, maxMusicVolumeBoss * (musicVolumeLevel / 6.0f));
    
    // Carregar sons SFX
    Sound defaultShootSound = LoadSound("assets/sounds/sfx/defaultshoot.wav");
    Sound dmgSound = LoadSound("assets/sounds/sfx/dmgsound.mp3");
    Sound deathSound = LoadSound("assets/sounds/sfx/deathsound.wav");
    Sound reloadSound = LoadSound("assets/sounds/sfx/reloadsound.wav");
    Sound menuButtonSound = LoadSound("assets/sounds/sfx/menusoundbuttons.wav");
    
    // Configurar volume inicial dos SFX (nível 2 de 6)
    float currentSFXVolume = maxSFXVolume * (sfxVolumeLevel / 6.0f);
    SetSoundVolume(defaultShootSound, currentSFXVolume);
    SetSoundVolume(dmgSound, currentSFXVolume);
    SetSoundVolume(deathSound, currentSFXVolume);
    SetSoundVolume(reloadSound, currentSFXVolume);
    SetSoundVolume(menuButtonSound, currentSFXVolume);
    
    Music* currentMusic = nullptr;
    
    int currentResolution = 0;
    const char* resolutions[] = { "1280x720", "1600x900", "1920x1080" };
    bool isFullscreen = false;
    int selectedSettingsOption = 0;
    const int settingsOptionsCount = 6;
    int settingsScrollOffset = 0;
    int statusScrollOffset = 0;
    bool cameFromPauseMenu = false;
    
    Player player;
    
    const float mapWidth = 2000.0f;
    const float mapHeight = 2000.0f;
    
    player.position = { mapWidth / 2, mapHeight / 2 };
    player.speed = 180.0f;
    player.radius = 15.0f;
    player.color = BLUE;
    player.maxHealth = 100;
    player.health = player.maxHealth;
    player.texture = playerTexture;
    player.rotation = 0.0f;
    player.maxAmmo = 8;
    player.ammo = player.maxAmmo;
    player.level = 1;
    player.xp = 0;
    player.xpToNextLevel = 100;
    player.coins = 0;
    
    int damageLvl = 1;
    int reloadSpeedLvl = 1;
    int bulletSpeedLvl = 1;
    int ammoCapacityLvl = 1;
    int healthLvl = 1;
    int fireRateLvl = 1;
    
    int damageCost = 100;
    int reloadSpeedCost = 100;
    int bulletSpeedCost = 100;
    int ammoCapacityCost = 100;
    int healthCost = 100;
    int fireRateCost = 100;
    
    int baseDamage = 5;
    float baseReloadTime = 1.2f;
    float baseBulletSpeed = 400.0f;
    int baseAmmo = 8;
    int baseHealth = 100;
    float baseFireRate = 0.5f;
    
    bool isReloading = false;
    float reloadTimer = 0.0f;
    float reloadDuration = baseReloadTime - ((reloadSpeedLvl - 1) * 0.1f);
    
    // Sistema de munições especiais
    AmmoItem ammoItems[4];
    
    ammoItems[0].rarity = AMMO_COMMON;
    ammoItems[0].damageMultiplier = 1.2f;
    ammoItems[0].texture = waterBulletTexture;
    ammoItems[0].name = "Water Ammo";
    ammoItems[0].color = SKYBLUE;
    
    ammoItems[1].rarity = AMMO_UNCOMMON;
    ammoItems[1].damageMultiplier = 1.5f;
    ammoItems[1].texture = fireBulletTexture;
    ammoItems[1].name = "Fire Ammo";
    ammoItems[1].color = ORANGE;
    
    ammoItems[2].rarity = AMMO_RARE;
    ammoItems[2].damageMultiplier = 1.8f;
    ammoItems[2].texture = acidBulletTexture;
    ammoItems[2].name = "Acid Ammo";
    ammoItems[2].color = LIME;
    
    ammoItems[3].rarity = AMMO_LEGENDARY;
    ammoItems[3].damageMultiplier = 2.0f;
    ammoItems[3].texture = magicBulletTexture;
    ammoItems[3].name = "Magic Ammo";
    ammoItems[3].color = PURPLE;
    
    int playerAmmoInventory[4] = { -1, -1, -1, -1 };
    int equippedAmmo = -1;
    int selectedInventorySlot = -1;
    
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    std::vector<Pickup> pickups;
    std::vector<Chest> chests;

    int score = 0;
    int wave = 1;
    int enemiesKilled = 0;
    int enemiesSpawned = 0;
    int enemiesPerWave = 10;
    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 5.0f;
    float pickupSpawnTimer = 0.0f;
    float pickupSpawnInterval = 15.0f;
    bool gameOver = false;
    bool isBossWave = false;
    bool bossSpawned = false;
    bool shouldQuit = false;
    int bossesDefeated = 0;
    float enemyDamageTimer = 0.0f;
    float enemyDamageInterval = 0.5f;
    float currentFireRate = baseFireRate - ((fireRateLvl - 1) * 0.05f);
    float fireRateCooldown = currentFireRate;
    float gameTime = 0.0f;
    int totalEnemiesKilled = 0;
    int topScore = 0;
    
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = { (float)screenWidth / 2, (float)screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.4f;
    
    while (!shouldQuit) {
        float deltaTime = GetFrameTime();
        
        if (currentMusic != nullptr) {
            UpdateMusicStream(*currentMusic);
        }
        
        // Verificar se a janela foi fechada (X ou Alt+F4)
        if (WindowShouldClose()) {
            shouldQuit = true;
            break;
        }
        
        // ===== MENU PRINCIPAL =====
        if (gameState == MENU) {
            // Tocar música do menu
            if (currentMusic != &menuMusic) {
                if (currentMusic != nullptr) StopMusicStream(*currentMusic);
                PlayMusicStream(menuMusic);
                currentMusic = &menuMusic;
            }
            
            // Navegação do menu
            if (IsKeyPressed(KEY_DOWN)) {
                selectedMenuOption = (selectedMenuOption + 1) % 5;
                if (!sfxMuted) PlaySound(menuButtonSound);
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedMenuOption = (selectedMenuOption - 1 + 5) % 5;
                if (!sfxMuted) PlaySound(menuButtonSound);
            }
            
            // Seleção de opção
            if (IsKeyPressed(KEY_ENTER)) {
                if (!sfxMuted) PlaySound(menuButtonSound);
                switch (selectedMenuOption) {
                    case 0:
                        gameState = PLAYING;
                        gameOver = false;
                        score = 0;
                        wave = 1;
                        enemiesKilled = 0;
                        enemiesSpawned = 0;
                        isBossWave = false;
                        bossSpawned = false;
                        bossesDefeated = 0;
                        enemySpawnInterval = 1.5f;
                        player.position = { mapWidth / 2, mapHeight / 2 };
                        player.health = player.maxHealth;
                        player.ammo = player.maxAmmo;
                        enemies.clear();
                        projectiles.clear();
                        enemySpawnTimer = 0.0f;
                        break;
                    case 1:
                        gameState = SHOP;
                        break;
                    case 2:
                        gameState = STATUS;
                        break;
                    case 3:
                        cameFromPauseMenu = false;
                        gameState = SETTINGS;
                        break;
                    case 4:
                        shouldQuit = true;
                        break;
                }
            }
        }
        
    // ===== LOJA =====
    else if (gameState == SHOP) {
        if (currentMusic != &menuMusic) {
            if (currentMusic != nullptr) StopMusicStream(*currentMusic);
            PlayMusicStream(menuMusic);
            currentMusic = &menuMusic;
        }
        
        if (IsKeyPressed(KEY_DOWN)) {
            selectedShopOption = (selectedShopOption + 1) % 6;
            if (!sfxMuted) PlaySound(menuButtonSound);
            int optionPositions[] = { 75, 220, 365, 510, 655, 800 };
            int targetY = optionPositions[selectedShopOption];
            int visibleBottom = 480;
            
            if (targetY > shopScrollOffset + visibleBottom) {
                shopScrollOffset = targetY - visibleBottom + 150;
                if (shopScrollOffset > 600) shopScrollOffset = 600;
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            selectedShopOption = (selectedShopOption - 1 + 6) % 6;
            if (!sfxMuted) PlaySound(menuButtonSound);
            int optionPositions[] = { 75, 220, 365, 510, 655, 800 };
            int targetY = optionPositions[selectedShopOption];
            
            if (targetY < shopScrollOffset) {
                shopScrollOffset = targetY - 50;
                if (shopScrollOffset < 0) shopScrollOffset = 0;
            }
        }
        
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            bool purchased = false;
            
            switch (selectedShopOption) {
                case 0:
                    if (player.coins >= damageCost) {
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= damageCost;
                        damageLvl++;
                        damageCost += 30; // Aumenta custo
                        purchased = true;
                    }
                    break;
                case 1: // Velocidade de Recarga
                    if (player.coins >= reloadSpeedCost && reloadSpeedLvl < 5) { // Max 5 níveis
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= reloadSpeedCost;
                        reloadSpeedLvl++;
                        reloadDuration = baseReloadTime - ((reloadSpeedLvl - 1) * 0.1f);
                        reloadSpeedCost += 30;
                        purchased = true;
                    }
                    break;
                case 2: // Velocidade da Bala
                    if (player.coins >= bulletSpeedCost) {
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= bulletSpeedCost;
                        bulletSpeedLvl++;
                        bulletSpeedCost += 30;
                        purchased = true;
                    }
                    break;
                case 3: // Capacidade de Munição
                    if (player.coins >= ammoCapacityCost) {
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= ammoCapacityCost;
                        ammoCapacityLvl++;
                        int newMaxAmmo = baseAmmo + ((ammoCapacityLvl - 1) * 2);
                        player.maxAmmo = newMaxAmmo;
                        ammoCapacityCost += 30;
                        purchased = true;
                    }
                    break;
                case 4: // Vida Máxima
                    if (player.coins >= healthCost) {
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= healthCost;
                        healthLvl++;
                        int newMaxHealth = baseHealth + ((healthLvl - 1) * 20);
                        int healthDiff = newMaxHealth - player.maxHealth;
                        player.maxHealth = newMaxHealth;
                        player.health += healthDiff; // Aumenta vida atual também
                        healthCost += 30;
                        purchased = true;
                    }
                    break;
                case 5:
                    if (player.coins >= fireRateCost && fireRateLvl < 8) {
                        if (!sfxMuted) PlaySound(menuButtonSound);
                        player.coins -= fireRateCost;
                        fireRateLvl++;
                        currentFireRate = baseFireRate - ((fireRateLvl - 1) * 0.05f);
                        fireRateCost += 30;
                        purchased = true;
                    }
                    break;
            }
        }
        
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_Q)) {
            gameState = MENU;
            selectedShopOption = 0;
            shopScrollOffset = 0;
        }
    }        else if (gameState == STATUS) {
            if (currentMusic != &menuMusic) {
                if (currentMusic != nullptr) StopMusicStream(*currentMusic);
                PlayMusicStream(menuMusic);
                currentMusic = &menuMusic;
            }
            
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                statusScrollOffset += 50;
                if (statusScrollOffset > 200) statusScrollOffset = 200;
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                statusScrollOffset -= 50;
                if (statusScrollOffset < 0) statusScrollOffset = 0;
            }
            
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_Q)) {
                gameState = MENU;
                statusScrollOffset = 0;
            }
        }
        
        else if (gameState == SETTINGS) {
            if (currentMusic != &menuMusic) {
                if (currentMusic != nullptr) StopMusicStream(*currentMusic);
                PlayMusicStream(menuMusic);
                currentMusic = &menuMusic;
            }
            
            if (IsKeyPressed(KEY_DOWN)) {
                selectedSettingsOption = (selectedSettingsOption + 1) % settingsOptionsCount;
                if (!sfxMuted) PlaySound(menuButtonSound);
                int optionPositions[] = { 75, 145, 315, 385, 585, 655 };
                int targetY = optionPositions[selectedSettingsOption];
                int visibleBottom = 480;
                
                if (targetY > settingsScrollOffset + visibleBottom) {
                    settingsScrollOffset = targetY - visibleBottom + 150;
                    if (settingsScrollOffset > 550) settingsScrollOffset = 550;
                }
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedSettingsOption = (selectedSettingsOption - 1 + settingsOptionsCount) % settingsOptionsCount;
                if (!sfxMuted) PlaySound(menuButtonSound);
                int optionPositions[] = { 75, 145, 315, 385, 585, 655 };
                int targetY = optionPositions[selectedSettingsOption];
                
                if (targetY < settingsScrollOffset) {
                    settingsScrollOffset = targetY - 50;
                    if (settingsScrollOffset < 0) settingsScrollOffset = 0;
                }
            }
            
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                if (!sfxMuted) PlaySound(menuButtonSound);
                switch (selectedSettingsOption) {
                    case 0:
                        if (IsKeyPressed(KEY_RIGHT)) {
                            currentResolution = (currentResolution + 1) % 3;
                        } else {
                            currentResolution = (currentResolution - 1 + 3) % 3;
                        }
                        break;
                    case 1:
                        isFullscreen = !isFullscreen;
                        ToggleFullscreen();
                        break;
                    case 2:
                        if (IsKeyPressed(KEY_RIGHT)) {
                            musicVolumeLevel = (musicVolumeLevel < 6) ? musicVolumeLevel + 1 : 6;
                        } else {
                            musicVolumeLevel = (musicVolumeLevel > 0) ? musicVolumeLevel - 1 : 0;
                        }
                        if (!musicMuted) {
                            float volumeScale = musicVolumeLevel / 6.0f;
                            SetMusicVolume(menuMusic, maxMusicVolumeMenu * volumeScale);
                            SetMusicVolume(levelMusic, maxMusicVolumeLevel * volumeScale);
                            SetMusicVolume(bossMusic, maxMusicVolumeBoss * volumeScale);
                        }
                        break;
                    case 3:
                        musicMuted = !musicMuted;
                        if (musicMuted) {
                            SetMusicVolume(menuMusic, 0.0f);
                            SetMusicVolume(levelMusic, 0.0f);
                            SetMusicVolume(bossMusic, 0.0f);
                        } else {
                            float volumeScale = musicVolumeLevel / 6.0f;
                            SetMusicVolume(menuMusic, maxMusicVolumeMenu * volumeScale);
                            SetMusicVolume(levelMusic, maxMusicVolumeLevel * volumeScale);
                            SetMusicVolume(bossMusic, maxMusicVolumeBoss * volumeScale);
                        }
                        break;
                    case 4:
                        if (IsKeyPressed(KEY_RIGHT)) {
                            sfxVolumeLevel = (sfxVolumeLevel < 6) ? sfxVolumeLevel + 1 : 6;
                        } else {
                            sfxVolumeLevel = (sfxVolumeLevel > 0) ? sfxVolumeLevel - 1 : 0;
                        }
                        currentSFXVolume = maxSFXVolume * (sfxVolumeLevel / 6.0f);
                        SetSoundVolume(defaultShootSound, currentSFXVolume);
                        SetSoundVolume(dmgSound, currentSFXVolume);
                        SetSoundVolume(deathSound, currentSFXVolume);
                        SetSoundVolume(reloadSound, currentSFXVolume);
                        SetSoundVolume(menuButtonSound, currentSFXVolume);
                        break;
                    case 5:
                        sfxMuted = !sfxMuted;
                        break;
                }
            }
            
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_Q)) {
                // Voltar para pause menu se estava jogando, senão menu principal
                if (cameFromPauseMenu) {
                    gameState = PAUSE_MENU;
                    cameFromPauseMenu = false;
                } else {
                    gameState = MENU;
                }
            }
        }
        
        // ===== MENU DE PAUSA =====
        else if (gameState == PAUSE_MENU) {
            if (IsKeyPressed(KEY_DOWN)) {
                selectedPauseOption = (selectedPauseOption + 1) % 4;
                if (!sfxMuted) PlaySound(menuButtonSound);
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedPauseOption = (selectedPauseOption - 1 + 4) % 4;
                if (!sfxMuted) PlaySound(menuButtonSound);
            }
            
            // Seleção de opção
            if (IsKeyPressed(KEY_ENTER)) {
                if (!sfxMuted) PlaySound(menuButtonSound);
                switch (selectedPauseOption) {
                    case 0:
                        gameState = PLAYING;
                        break;
                    case 1:
                        cameFromPauseMenu = true;
                        gameState = SETTINGS;
                        break;
                    case 2:
                        gameState = MENU;
                        selectedMenuOption = 0;
                        break;
                    case 3:
                        shouldQuit = true;
                        break;
                }
            }
            
            if (IsKeyPressed(KEY_P)) {
                gameState = PLAYING;
            }
            
            if (WindowShouldClose()) {
                shouldQuit = true;
            }
        }
        
        else if (gameState == INVENTORY) {
            if (IsKeyPressed(KEY_RIGHT)) {
                selectedInventorySlot++;
                if (selectedInventorySlot >= 8) selectedInventorySlot = 0;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                selectedInventorySlot--;
                if (selectedInventorySlot < 0) selectedInventorySlot = 7;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                selectedInventorySlot += 4;
                if (selectedInventorySlot >= 8) selectedInventorySlot -= 8;
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedInventorySlot -= 4;
                if (selectedInventorySlot < 0) selectedInventorySlot += 8;
            }
            
            if (IsKeyPressed(KEY_ENTER) && selectedInventorySlot >= 0 && selectedInventorySlot < 4) {
                if (playerAmmoInventory[selectedInventorySlot] != -1) {
                    equippedAmmo = selectedInventorySlot;
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                equippedAmmo = -1;
            }
            
            if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_B)) {
                gameState = PLAYING;
            }
            
            if (WindowShouldClose()) {
                shouldQuit = true;
            }
        }
        
        else if (gameState == CHEAT_MENU) {
            if (IsKeyPressed(KEY_DOWN)) {
                selectedCheatOption = (selectedCheatOption + 1) % 2;
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedCheatOption = (selectedCheatOption - 1 + 2) % 2;
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                switch (selectedCheatOption) {
                    case 0:
                        player.coins += 100000;
                        break;
                    case 1:
                        if (!isBossWave && !gameOver) {
                            isBossWave = true;
                            bossSpawned = false;
                            enemies.clear();
                            enemiesKilled = 0;
                            enemiesSpawned = 0;
                        }
                        break;
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_HOME)) {
                gameState = PLAYING;
            }
            
            if (WindowShouldClose()) {
                shouldQuit = true;
            }
        }
        
        else if (gameState == PLAYING && !gameOver) {
            gameTime += deltaTime;
            
            Music* targetMusic = isBossWave ? &bossMusic : &levelMusic;
            if (currentMusic != targetMusic) {
                if (currentMusic != nullptr) StopMusicStream(*currentMusic);
                PlayMusicStream(*targetMusic);
                currentMusic = targetMusic;
            }
            
            if (fireRateCooldown > 0) {
                fireRateCooldown -= deltaTime;
            }
            
            camera.target = player.position;
            
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
                gameState = PAUSE_MENU;
                selectedPauseOption = 0;
            }
            
            if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_B)) {
                gameState = INVENTORY;
            }
            
            if (IsKeyPressed(KEY_HOME)) {
                gameState = CHEAT_MENU;
                selectedCheatOption = 0;
            }
            
            if (WindowShouldClose()) {
                shouldQuit = true;
            }
            
            Vector2 mouseScreenPos = GetMousePosition();
            Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
            float dx = mouseWorldPos.x - player.position.x;
            float dy = mouseWorldPos.y - player.position.y;
            player.rotation = atan2f(dy, dx) * RAD2DEG;
            
            Vector2 movement = { 0, 0 };
            
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.y -= 1;
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.y += 1;
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x -= 1;
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x += 1;
            
            float length = sqrt(movement.x * movement.x + movement.y * movement.y);
            if (length > 0) {
                movement.x /= length;
                movement.y /= length;
            }
            
            player.position.x += movement.x * player.speed * deltaTime;
            player.position.y += movement.y * player.speed * deltaTime;
            
            if (player.position.x - player.radius < 0) player.position.x = player.radius;
            if (player.position.x + player.radius > mapWidth) player.position.x = mapWidth - player.radius;
            if (player.position.y - player.radius < 0) player.position.y = player.radius;
            if (player.position.y + player.radius > mapHeight) player.position.y = mapHeight - player.radius;
            
            if (isReloading) {
                reloadTimer += deltaTime;
                player.texture = playerReloadTexture;
                
                if (reloadTimer >= reloadDuration) {
                    player.ammo = player.maxAmmo;
                    isReloading = false;
                    reloadTimer = 0.0f;
                    player.texture = playerTexture;
                }
            } else {
                player.texture = playerTexture;
            }
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && player.ammo > 0 && !isReloading && fireRateCooldown <= 0) {
                Projectile proj;
                proj.position = player.position;
                proj.radius = 5.0f;
                proj.active = true;
                
                int finalDamage = baseDamage + ((damageLvl - 1) * 3);
                
                if (equippedAmmo >= 0 && equippedAmmo < 4 && playerAmmoInventory[equippedAmmo] != -1) {
                    int ammoItemIndex = playerAmmoInventory[equippedAmmo];
                    finalDamage = (int)(finalDamage * ammoItems[ammoItemIndex].damageMultiplier);
                    proj.texture = ammoItems[ammoItemIndex].texture;
                } else {
                    proj.texture = bulletTexture;
                }
                
                proj.damage = finalDamage;
                
                Vector2 mouseScreenPos = GetMousePosition();
                Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
                Vector2 direction = { mouseWorldPos.x - player.position.x, mouseWorldPos.y - player.position.y };
                float len = sqrt(direction.x * direction.x + direction.y * direction.y);
                
                if (len > 0) {
                    direction.x /= len;
                    direction.y /= len;
                }
                
                proj.rotation = atan2f(direction.y, direction.x) * RAD2DEG;
                float bulletSpeed = baseBulletSpeed + ((bulletSpeedLvl - 1) * 50.0f);
                proj.velocity = { direction.x * bulletSpeed, direction.y * bulletSpeed };
                projectiles.push_back(proj);
                player.ammo--;
                
                currentFireRate = baseFireRate - ((fireRateLvl - 1) * 0.05f);
                fireRateCooldown = currentFireRate;
                
                if (!sfxMuted) {
                        PlaySound(defaultShootSound);
                    }
                }
            
            if (IsKeyPressed(KEY_R) && !isReloading && player.ammo < player.maxAmmo) {
                isReloading = true;
                reloadTimer = 0.0f;
                if (!sfxMuted) PlaySound(reloadSound);
            }
            
            for (size_t i = 0; i < projectiles.size(); i++) {
                if (projectiles[i].active) {
                    projectiles[i].position.x += projectiles[i].velocity.x * deltaTime;
                    projectiles[i].position.y += projectiles[i].velocity.y * deltaTime;
                    
                    if (projectiles[i].position.x < -100 || projectiles[i].position.x > mapWidth + 100 ||
                        projectiles[i].position.y < -100 || projectiles[i].position.y > mapHeight + 100) {
                        projectiles[i].active = false;
                    }
                }
            }
            
            enemySpawnTimer += deltaTime;
            
            if (wave % 6 == 0 && !isBossWave) {
                isBossWave = true;
                bossSpawned = false;
                enemies.clear();
                enemiesKilled = 0;
                enemiesSpawned = 0;
            }
            
            if (isBossWave) {
                if (!bossSpawned) {
                    Enemy boss;
                    boss.radius = 60.0f;
                    boss.speed = 50.0f + (bossesDefeated * 3.0f);
                    boss.color = PURPLE;
                    boss.active = true;
                    boss.maxHealth = 150 + (bossesDefeated * 20);
                    boss.health = boss.maxHealth;
                    boss.isBoss = true;
                    boss.position = { mapWidth / 2, 100.0f };
                    boss.texture = enemyTexture;
                    boss.rotation = 90.0f;
                    boss.xpDrop = 100 + (bossesDefeated * 20); // XP aumenta com dificuldade
                    boss.coinDrop = 50 + (bossesDefeated * 10); // Moedas aumentam com dificuldade
                    enemies.push_back(boss);
                    bossSpawned = true;
                }
                
                // Verificar se boss foi derrotado
                if (bossSpawned && enemies.empty()) {
                    isBossWave = false;
                    bossSpawned = false;
                    bossesDefeated++; // Incrementar contador de bosses
                    wave++;
                    enemiesKilled = 0;
                    enemiesSpawned = 0;
                    score += 100 + (bossesDefeated * 50);
                    
                    // Spawnar baú próximo ao jogador após derrotar o boss
                    Chest chest;
                    chest.radius = 40.0f;
                    chest.active = true;
                    chest.state = CHEST_CLOSED;
                    chest.texture = chestTextures[0];
                    chest.currentFrame = 0;
                    chest.animTimer = 0.0f;
                    chest.itemRevealed = false;
                    chest.revealTimer = 0.0f;
                    
                    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                    float distance = (float)GetRandomValue(100, 200);
                    chest.position = {
                        player.position.x + cosf(angle) * distance,
                        player.position.y + sinf(angle) * distance
                    };
                    
                    int randValue = GetRandomValue(1, 100);
                    if (randValue <= 5) {
                        chest.itemDrop = AMMO_LEGENDARY;
                    } else if (randValue <= 15) {
                        chest.itemDrop = AMMO_RARE;
                    } else if (randValue <= 40) {
                        chest.itemDrop = AMMO_UNCOMMON;
                    } else {
                        chest.itemDrop = AMMO_COMMON;
                    }
                    
                    chests.push_back(chest);
                }
            } else {
                if (enemySpawnTimer >= enemySpawnInterval && enemiesSpawned < enemiesPerWave) {
                    enemySpawnTimer = 0.0f;
                    
                    int remaining = enemiesPerWave - enemiesSpawned;
                    int spawnCount = (remaining >= 2) ? GetRandomValue(1, 2) : remaining;
                    
                    for (int s = 0; s < spawnCount; s++) {
                        Enemy enemy;
                        enemy.radius = 30.0f;
                        enemy.speed = 70.0f + (bossesDefeated * 10.0f);
                        enemy.color = RED;
                        enemy.active = true;
                        enemy.maxHealth = 30 + (bossesDefeated * 15);
                        enemy.health = enemy.maxHealth;
                        enemy.isBoss = false;
                        enemy.texture = enemyTexture;
                        enemy.rotation = 0.0f;
                        enemy.xpDrop = 10 + (bossesDefeated * 2);
                        enemy.coinDrop = 5 + (bossesDefeated * 2);
                        
                        // Spawn em posição aleatória nas bordas do mapa
                        int side = GetRandomValue(0, 3);
                        switch (side) {
                            case 0:
                                enemy.position = { (float)GetRandomValue(0, (int)mapWidth), -enemy.radius };
                                break;
                            case 1:
                                enemy.position = { mapWidth + enemy.radius, (float)GetRandomValue(0, (int)mapHeight) };
                                break;
                            case 2:
                                enemy.position = { (float)GetRandomValue(0, (int)mapWidth), mapHeight + enemy.radius };
                                break;
                            case 3:
                                enemy.position = { -enemy.radius, (float)GetRandomValue(0, (int)mapHeight) };
                                break;
                        }
                        
                        enemies.push_back(enemy);
                        enemiesSpawned++;
                    }
                }
                
                if (enemiesSpawned >= enemiesPerWave && enemiesKilled >= enemiesPerWave) {
                    wave++;
                    enemiesKilled = 0;
                    enemiesSpawned = 0;
                    score += 50;
                    enemySpawnInterval = fmax(0.3f, 1.5f - (wave * 0.04f));
                }
            }
            
            pickupSpawnTimer += deltaTime;
            if (pickupSpawnTimer >= pickupSpawnInterval) {
                pickupSpawnTimer = 0.0f;
                
                // Chance de 70% de spawnar um pickup
                if (GetRandomValue(0, 100) < 70) {
                    Pickup pickup;
                    pickup.radius = 20.0f;
                    pickup.active = true;
                    
                    // 50% chance de ser vida, 50% loot
                    pickup.type = (GetRandomValue(0, 1) == 0) ? PICKUP_HEALTH : PICKUP_LOOT;
                    pickup.texture = (pickup.type == PICKUP_HEALTH) ? healthPickupTexture : lootPickupTexture;
                    
                    pickup.position = {
                        (float)GetRandomValue(200, (int)mapWidth - 200),
                        (float)GetRandomValue(200, (int)mapHeight - 200)
                    };
                    
                    pickups.push_back(pickup);
                }
            }
            
            for (size_t i = 0; i < pickups.size(); i++) {
                if (pickups[i].active) {
                    if (CheckCollisionCircles(player.position, player.radius,
                                             pickups[i].position, pickups[i].radius)) {
                        pickups[i].active = false;
                        
                        if (pickups[i].type == PICKUP_HEALTH) {
                            player.health += 30;
                            if (player.health > player.maxHealth) {
                                player.health = player.maxHealth;
                            }
                        } else {
                            if (GetRandomValue(0, 1) == 0) {
                                int xpGain = GetRandomValue(20, 40);
                                player.xp += xpGain;
                                
                                while (player.xp >= player.xpToNextLevel) {
                                    player.xp -= player.xpToNextLevel;
                                    player.level++;
                                    player.xpToNextLevel = player.level * 100;
                                }
                            } else {
                                player.coins += GetRandomValue(10, 25);
                            }
                        }
                    }
                }
            }
            
            pickups.erase(
                std::remove_if(pickups.begin(), pickups.end(),
                    [](const Pickup& p) { return !p.active; }),
                pickups.end()
            );
            
            for (size_t i = 0; i < chests.size(); i++) {
                if (chests[i].active) {
                    float distToChest = sqrt(
                        (chests[i].position.x - player.position.x) * (chests[i].position.x - player.position.x) +
                        (chests[i].position.y - player.position.y) * (chests[i].position.y - player.position.y)
                    );
                    
                    if (distToChest < (chests[i].radius + player.radius + 30.0f) && chests[i].state == CHEST_CLOSED) {
                        if (IsKeyPressed(KEY_E)) {
                            chests[i].state = CHEST_OPENING;
                            chests[i].animTimer = 0.0f;
                        }
                    }
                    
                    if (chests[i].state == CHEST_OPENING) {
                        chests[i].animTimer += deltaTime;
                        
                        int targetFrame = (int)(chests[i].animTimer / 0.1f);
                        if (targetFrame > 5) targetFrame = 5;
                        
                        chests[i].currentFrame = targetFrame;
                        chests[i].texture = chestTextures[targetFrame];
                        
                        if (chests[i].animTimer >= 0.6f) {
                            chests[i].state = CHEST_OPENED;
                            chests[i].revealTimer = 0.0f;
                            
                            int itemIndex = (int)chests[i].itemDrop;
                            
                            bool alreadyHas = false;
                            for (int slot = 0; slot < 4; slot++) {
                                if (playerAmmoInventory[slot] == itemIndex) {
                                    alreadyHas = true;
                                    break;
                                }
                            }
                            
                            if (alreadyHas) {
                                // Converter em moedas (50-150 baseado na raridade)
                                int coinReward = 50 + (itemIndex * 35); // Common: 50, Uncommon: 85, Rare: 120, Legendary: 155
                                player.coins += coinReward;
                            } else {
                                // Adicionar munição ao primeiro slot vazio
                                for (int slot = 0; slot < 4; slot++) {
                                    if (playerAmmoInventory[slot] == -1) {
                                        playerAmmoInventory[slot] = itemIndex;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    
                    // Revelação do item (mostrar por 3 segundos)
                    if (chests[i].state == CHEST_OPENED) {
                        chests[i].revealTimer += deltaTime;
                        chests[i].itemRevealed = true;
                        
                        // Remover baú após 5 segundos
                        if (chests[i].revealTimer >= 5.0f) {
                            chests[i].active = false;
                        }
                    }
                }
            }
            
            // Remover baús inativos
            chests.erase(
                std::remove_if(chests.begin(), chests.end(),
                    [](const Chest& c) { return !c.active; }),
                chests.end()
            );
            
            // Atualizar inimigos e verificar colisões
            bool anyEnemyTouchingPlayer = false;
            
            for (size_t i = 0; i < enemies.size(); i++) {
                if (enemies[i].active) {
                    // Mover em direção ao jogador
                    Vector2 direction = { player.position.x - enemies[i].position.x, 
                                         player.position.y - enemies[i].position.y };
                    float len = sqrt(direction.x * direction.x + direction.y * direction.y);
                    
                    if (len > 0) {
                        direction.x /= len;
                        direction.y /= len;
                    }
                    
                    // Calcular rotação do inimigo
                    enemies[i].rotation = atan2f(direction.y, direction.x) * RAD2DEG;
                    
                    // Movimento do inimigo
                    Vector2 newPos = {
                        enemies[i].position.x + direction.x * enemies[i].speed * deltaTime,
                        enemies[i].position.y + direction.y * enemies[i].speed * deltaTime
                    };
                    
                    // Verificar colisão com jogador ANTES de mover
                    float distToPlayer = sqrt(
                        (newPos.x - player.position.x) * (newPos.x - player.position.x) +
                        (newPos.y - player.position.y) * (newPos.y - player.position.y)
                    );
                    
                    // Se colidiu com jogador, empurrar inimigo para trás um pouco
                    if (distToPlayer < (enemies[i].radius + player.radius)) {
                        anyEnemyTouchingPlayer = true;
                        
                        // Não deixar inimigo atravessar o jogador
                        // Posicionar inimigo na borda do círculo do jogador
                        float targetDist = enemies[i].radius + player.radius + 1.0f;
                        if (len > 0) {
                            enemies[i].position.x = player.position.x - direction.x * targetDist;
                            enemies[i].position.y = player.position.y - direction.y * targetDist;
                        }
                    } else {
                        // Mover normalmente se não colidiu
                        enemies[i].position = newPos;
                    }
                    
                    // Colisão entre inimigos (evitar overlap)
                    for (size_t j = i + 1; j < enemies.size(); j++) {
                        if (enemies[j].active) {
                            float dx = enemies[j].position.x - enemies[i].position.x;
                            float dy = enemies[j].position.y - enemies[i].position.y;
                            float dist = sqrt(dx * dx + dy * dy);
                            float minDist = enemies[i].radius + enemies[j].radius;
                            
                            if (dist < minDist && dist > 0) {
                                // Empurrar inimigos para longe um do outro
                                float overlap = (minDist - dist) * 0.5f;
                                float nx = dx / dist;
                                float ny = dy / dist;
                                
                                enemies[i].position.x -= nx * overlap;
                                enemies[i].position.y -= ny * overlap;
                                enemies[j].position.x += nx * overlap;
                                enemies[j].position.y += ny * overlap;
                            }
                        }
                    }
                }
            }
            
            // Sistema de dano contínuo quando inimigo está tocando jogador
            if (anyEnemyTouchingPlayer) {
                enemyDamageTimer += deltaTime;
                if (enemyDamageTimer >= enemyDamageInterval) {
                    int damage = 10; // Dano base dos inimigos
                    
                    // Verificar se há boss tocando para aumentar dano
                    for (size_t i = 0; i < enemies.size(); i++) {
                        if (enemies[i].active && enemies[i].isBoss) {
                            float dist = sqrt(
                                (enemies[i].position.x - player.position.x) * (enemies[i].position.x - player.position.x) +
                                (enemies[i].position.y - player.position.y) * (enemies[i].position.y - player.position.y)
                            );
                            if (dist < (enemies[i].radius + player.radius)) {
                                damage = 15; // Boss causa mais dano
                                break;
                            }
                        }
                    }
                    
                    player.health -= damage;
                    enemyDamageTimer = 0.0f;
                    
                    // Tocar som de dano
                    if (!sfxMuted) PlaySound(dmgSound);
                    
                    if (player.health <= 0) {
                        gameOver = true;
                        player.health = 0;
                        if (!sfxMuted) PlaySound(deathSound);
                    }
                }
            } else {
                // Resetar timer quando não há colisão
                enemyDamageTimer = 0.0f;
            }
            
            // Verificar colisões projéteis-inimigos
            for (size_t i = 0; i < projectiles.size(); i++) {
                if (projectiles[i].active) {
                    for (size_t j = 0; j < enemies.size(); j++) {
                        if (enemies[j].active) {
                            if (CheckCollisionCircles(projectiles[i].position, projectiles[i].radius,
                                                     enemies[j].position, enemies[j].radius)) {
                                projectiles[i].active = false;
                                enemies[j].health -= projectiles[i].damage;
                                
                                // Verificar se inimigo morreu
                                if (enemies[j].health <= 0) {
                                    enemies[j].active = false;
                                    enemiesKilled++;
                                    totalEnemiesKilled++; // Incrementar contador total
                                    
                                    // Tocar som de morte
                                    if (!sfxMuted) PlaySound(deathSound);
                                    
                                    // Ganhar XP e moedas
                                    player.xp += enemies[j].xpDrop;
                                    player.coins += enemies[j].coinDrop;
                                    
                                    // Level up
                                    while (player.xp >= player.xpToNextLevel) {
                                        player.xp -= player.xpToNextLevel;
                                        player.level++;
                                        player.xpToNextLevel = player.level * 100; // XP necessário aumenta com nível
                                    }
                                    
                                    if (enemies[j].isBoss) {
                                        score += 200; // Pontos extras pelo boss
                                    } else {
                                        score += 10 + (wave * 2); // Mais pontos em waves altas
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // Limpar vetores de objetos inativos
            projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& p) { return !p.active; }), projectiles.end());
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.active; }), enemies.end());
        }
        
        // ===== GAME OVER =====
        else if (gameState == PLAYING && gameOver) {
            // Reiniciar jogo
            if (IsKeyPressed(KEY_SPACE)) {
                gameOver = false;
                score = 0;
                wave = 1;
                enemiesKilled = 0;
                enemiesSpawned = 0;
                isBossWave = false;
                bossSpawned = false;
                enemySpawnInterval = 1.5f;
                player.position = { (float)screenWidth / 2, (float)screenHeight / 2 };
                player.health = player.maxHealth;
                player.ammo = player.maxAmmo;
                enemies.clear();
                projectiles.clear();
                enemySpawnTimer = 0.0f;
            }
            
            // Voltar ao menu
            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_BACKSPACE)) {
                gameState = MENU;
                selectedMenuOption = 0;
            }
            
            if (WindowShouldClose()) {
                shouldQuit = true;
            }
        }
        
        // Renderização
        BeginDrawing();
        ClearBackground(DARKGRAY);
        
        // ===== RENDERIZAR MENU PRINCIPAL =====
        if (gameState == MENU) {
            // Desenhar background
            Rectangle bgSrc = { 0, 0, (float)bgMenu.width, (float)bgMenu.height };
            Rectangle bgDst = { 0, 0, (float)screenWidth, (float)screenHeight };
            DrawTexturePro(bgMenu, bgSrc, bgDst, {0, 0}, 0.0f, WHITE);
            
            // Título
            DrawText("TOP-DOWN SHOOTER", screenWidth / 2 - 200, 80, 40, WHITE);
            DrawText("POO2 - Projeto", screenWidth / 2 - 120, 130, 20, GRAY);
            
            // Opções do menu
            const char* menuOptions[] = { "JOGAR", "LOJA", "STATUS", "CONFIGURACOES", "SAIR" };
            int startY = 220;
            int spacing = 60;
            
            for (int i = 0; i < 5; i++) {
                Color textColor = (i == selectedMenuOption) ? YELLOW : WHITE;
                int fontSize = (i == selectedMenuOption) ? 30 : 25;
                int textWidth = MeasureText(menuOptions[i], fontSize);
                
                if (i == selectedMenuOption) {
                    DrawText(">", screenWidth / 2 - textWidth / 2 - 30, startY + (i * spacing), fontSize, YELLOW);
                }
                
                DrawText(menuOptions[i], screenWidth / 2 - textWidth / 2, startY + (i * spacing), fontSize, textColor);
            }
            
            // Instruções
            DrawText("Use SETAS para navegar | ENTER para selecionar", screenWidth / 2 - 230, screenHeight - 40, 16, DARKGRAY);
        }
        
        // ===== RENDERIZAR LOJA =====
        else if (gameState == SHOP) {
            // Desenhar background
            Rectangle bgSrc = { 0, 0, (float)bgLightBrick.width, (float)bgLightBrick.height };
            Rectangle bgDst = { 0, 0, (float)screenWidth, (float)screenHeight };
            DrawTexturePro(bgLightBrick, bgSrc, bgDst, {0, 0}, 0.0f, WHITE);
            
            // Título fixo no topo
            float titleScale = 2.5f;
            int titleX = screenWidth / 2 - 80;
            int titleY = 30;
            DrawText("LOJA DE UPGRADES", titleX, titleY, 32, GOLD);
            
            // Mostrar moedas e nível do jogador no topo
            DrawText(TextFormat("MOEDAS: %d", player.coins), 50, 30, 24, YELLOW);
            DrawText(TextFormat("NIVEL: %d", player.level), screenWidth - 180, 30, 24, SKYBLUE);
            DrawText(TextFormat("XP: %d/%d", player.xp, player.xpToNextLevel), screenWidth - 180, 55, 18, LIGHTGRAY);
            
            // Área de conteúdo com scroll
            int contentStartY = 120;
            int baseY = contentStartY - shopScrollOffset;
            int sectionWidth = 1000;
            int sectionX = (screenWidth - sectionWidth) / 2;
            
            // Array com informações dos upgrades
            const char* upgradeNames[] = { "DANO", "VELOCIDADE DE RECARGA", "VELOCIDADE DA BALA", "CAPACIDADE DE MUNICAO", "VIDA MAXIMA", "VELOCIDADE DE DISPARO" };
            int upgradeLevels[] = { damageLvl, reloadSpeedLvl, bulletSpeedLvl, ammoCapacityLvl, healthLvl, fireRateLvl };
            int upgradeCosts[] = { damageCost, reloadSpeedCost, bulletSpeedCost, ammoCapacityCost, healthCost, fireRateCost };
            const char* upgradeValues[] = {
                TextFormat("%d (+3/lvl)", baseDamage + ((damageLvl - 1) * 3)),
                TextFormat("%.1fs (-0.1s/lvl)", baseReloadTime - ((reloadSpeedLvl - 1) * 0.1f)),
                TextFormat("%.0f (+50/lvl)", baseBulletSpeed + ((bulletSpeedLvl - 1) * 50.0f)),
                TextFormat("%d (+2/lvl)", baseAmmo + ((ammoCapacityLvl - 1) * 2)),
                TextFormat("%d (+20/lvl)", baseHealth + ((healthLvl - 1) * 20)),
                TextFormat("%.2fs (-0.05s/lvl)", baseFireRate - ((fireRateLvl - 1) * 0.05f))
            };
            
            // Renderizar cada upgrade
            for (int i = 0; i < 6; i++) {
                int upgradeY = baseY + (i * 145);
                
                // Background do upgrade
                Rectangle bgSrc = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
                Rectangle bgDst = { (float)sectionX, (float)upgradeY, (float)sectionWidth, 130.0f };
                DrawTexturePro(whiteSquareUI, bgSrc, bgDst, {0, 0}, 0.0f, WHITE);
                
                // Borda (destacar se selecionado)
                Color borderColor = (i == selectedShopOption) ? GOLD : BLACK;
                float borderThickness = (i == selectedShopOption) ? 5.0f : 3.0f;
                DrawRectangleLinesEx({ (float)sectionX, (float)upgradeY, (float)sectionWidth, 130.0f }, borderThickness, borderColor);
                
                // Nome do upgrade
                DrawText(upgradeNames[i], sectionX + 20, upgradeY + 15, 26, BLACK);
                
                // Nível atual
                DrawText(TextFormat("NIVEL: %d", upgradeLevels[i]), sectionX + 20, upgradeY + 50, 20, DARKGRAY);
                
                // Valor atual
                DrawText(TextFormat("VALOR: %s", upgradeValues[i]), sectionX + 20, upgradeY + 80, 20, DARKBLUE);
                
                // Custo do próximo upgrade
                bool canAfford = player.coins >= upgradeCosts[i];
                bool isMaxed = (i == 1 && reloadSpeedLvl >= 5) || (i == 5 && fireRateLvl >= 8); // Recarga e fire rate têm limite
                
                if (isMaxed) {
                    DrawText("MAXIMO", sectionX + 700, upgradeY + 50, 24, RED);
                } else {
                    Color costColor = canAfford ? GREEN : RED;
                    DrawText(TextFormat("CUSTO: %d", upgradeCosts[i]), sectionX + 700, upgradeY + 50, 22, costColor);
                    
                    // Ícone de moeda
                    DrawCircle(sectionX + 690, upgradeY + 62, 8, GOLD);
                }
                
                // Instrução de compra (se selecionado)
                if (i == selectedShopOption) {
                    if (!isMaxed) {
                        DrawText(canAfford ? "PRESSIONE ENTER/ESPACO PARA COMPRAR" : "MOEDAS INSUFICIENTES", 
                                sectionX + 20, upgradeY + 105, 16, canAfford ? DARKGREEN : MAROON);
                    }
                }
            }
            
            // Rodapé fixo com instruções
            DrawRectangle(0, screenHeight - 80, screenWidth, 80, Fade(BLACK, 0.8f));
            DrawText("SETAS: Navegar | ENTER/ESPACO: Comprar | Q/BACKSPACE: Voltar ao Menu", 
                    screenWidth / 2 - 350, screenHeight - 50, 18, WHITE);
        }
        
        // ===== RENDERIZAR STATUS =====
        else if (gameState == STATUS) {
            // Desenhar background
            Rectangle bgSrc = { 0, 0, (float)bgLightBrick.width, (float)bgLightBrick.height };
            Rectangle bgDst = { 0, 0, (float)screenWidth, (float)screenHeight };
            DrawTexturePro(bgLightBrick, bgSrc, bgDst, {0, 0}, 0.0f, WHITE);
            
            // Título fixo no topo
            DrawText("ESTATISTICAS DO JOGADOR", screenWidth / 2 - 250, 30, 36, BLACK);
            
            // Área de conteúdo com scroll
            int contentStartY = 100 - statusScrollOffset;
            int sectionWidth = 1000;
            int sectionX = (screenWidth - sectionWidth) / 2;
            
            // ===== SEÇÃO 1: INFORMAÇÕES BÁSICAS =====
            int section1Y = contentStartY;
            int section1Height = 180;
            
            // Background branco da seção
            Rectangle whiteBgSrc1 = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
            Rectangle whiteBgDst1 = { (float)sectionX, (float)section1Y, (float)sectionWidth, (float)section1Height };
            DrawTexturePro(whiteSquareUI, whiteBgSrc1, whiteBgDst1, {0, 0}, 0.0f, WHITE);
            DrawRectangleLinesEx({ (float)sectionX, (float)section1Y, (float)sectionWidth, (float)section1Height }, 4, BLACK);
            
            DrawText("INFORMACOES GERAIS", sectionX + sectionWidth / 2 - 180, section1Y + 15, 28, BLACK);
            
            // Nível
            DrawText(TextFormat("NIVEL: %d", player.level), sectionX + 30, section1Y + 60, 24, DARKBLUE);
            DrawText(TextFormat("XP: %d / %d", player.xp, player.xpToNextLevel), sectionX + 300, section1Y + 60, 24, DARKBLUE);
            
            // Top Score
            DrawText(TextFormat("TOP SCORE: %d", topScore), sectionX + 650, section1Y + 60, 24, GOLD);
            
            // Moedas com ícone
            DrawText("MOEDAS:", sectionX + 30, section1Y + 100, 24, DARKGREEN);
            DrawCircle(sectionX + 170, section1Y + 112, 12, GOLD);
            DrawText(TextFormat("%d", player.coins), sectionX + 190, section1Y + 100, 24, DARKGREEN);
            
            // Tempo jogado (converter para minutos:segundos)
            int minutes = (int)(gameTime / 60);
            int seconds = (int)(gameTime) % 60;
            DrawText(TextFormat("TEMPO JOGADO: %02d:%02d", minutes, seconds), sectionX + 30, section1Y + 140, 24, DARKPURPLE);
            
            // Inimigos abatidos
            DrawText(TextFormat("INIMIGOS ABATIDOS: %d", totalEnemiesKilled), sectionX + 500, section1Y + 140, 24, RED);
            
            // ===== SEÇÃO 2: ATRIBUTOS =====
            int section2Y = section1Y + section1Height + 30; // Aumentado espaçamento de 20 para 30
            int section2Height = 380;
            
            Rectangle whiteBgSrc2 = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
            Rectangle whiteBgDst2 = { (float)sectionX, (float)section2Y, (float)sectionWidth, (float)section2Height };
            DrawTexturePro(whiteSquareUI, whiteBgSrc2, whiteBgDst2, {0, 0}, 0.0f, WHITE);
            DrawRectangleLinesEx({ (float)sectionX, (float)section2Y, (float)sectionWidth, (float)section2Height }, 4, BLACK);
            
            DrawText("ATRIBUTOS", sectionX + sectionWidth / 2 - 100, section2Y + 15, 28, BLACK);
            
            // Calcular valores atuais dos atributos
            int currentDamage = baseDamage + ((damageLvl - 1) * 3);
            float currentReloadTime = baseReloadTime - ((reloadSpeedLvl - 1) * 0.1f);
            float currentBulletSpeed = baseBulletSpeed + ((bulletSpeedLvl - 1) * 50.0f);
            int currentMaxAmmo = baseAmmo + ((ammoCapacityLvl - 1) * 2);
            int currentMaxHealth = baseHealth + ((healthLvl - 1) * 20);
            float currentFireRateValue = baseFireRate - ((fireRateLvl - 1) * 0.05f);
            
            int attrY = section2Y + 60;
            int attrSpacing = 50;
            
            // Dano (centralizado)
            DrawText(TextFormat("DANO:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", damageLvl), sectionX + 370, attrY, 22, DARKBLUE);
            DrawText(TextFormat("Valor: %d", currentDamage), sectionX + 600, attrY, 22, MAROON);
            attrY += attrSpacing;
            
            // Velocidade de Recarga
            DrawText(TextFormat("VELOCIDADE DE RECARGA:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", reloadSpeedLvl), sectionX + 450, attrY, 22, DARKBLUE);
            DrawText(TextFormat("%.1fs", currentReloadTime), sectionX + 650, attrY, 22, MAROON);
            attrY += attrSpacing;
            
            // Velocidade da Bala
            DrawText(TextFormat("VELOCIDADE DA BALA:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", bulletSpeedLvl), sectionX + 450, attrY, 22, DARKBLUE);
            DrawText(TextFormat("%.0f", currentBulletSpeed), sectionX + 650, attrY, 22, MAROON);
            attrY += attrSpacing;
            
            // Capacidade de Munição
            DrawText(TextFormat("CAPACIDADE DE MUNICAO:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", ammoCapacityLvl), sectionX + 450, attrY, 22, DARKBLUE);
            DrawText(TextFormat("%d", currentMaxAmmo), sectionX + 650, attrY, 22, MAROON);
            attrY += attrSpacing;
            
            // Vida Máxima
            DrawText(TextFormat("VIDA MAXIMA:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", healthLvl), sectionX + 450, attrY, 22, DARKBLUE);
            DrawText(TextFormat("%d", currentMaxHealth), sectionX + 650, attrY, 22, MAROON);
            attrY += attrSpacing;
            
            // Velocidade de Disparo
            DrawText(TextFormat("VELOCIDADE DE DISPARO:"), sectionX + 30, attrY, 22, DARKGRAY);
            DrawText(TextFormat("Nivel %d", fireRateLvl), sectionX + 450, attrY, 22, DARKBLUE);
            DrawText(TextFormat("%.2fs", currentFireRateValue), sectionX + 650, attrY, 22, MAROON);
            
            // Rodapé com instruções
            DrawRectangle(0, screenHeight - 80, screenWidth, 80, Fade(BLACK, 0.8f));
            DrawText("SETAS/WASD: Rolar | Q/BACKSPACE: Voltar ao Menu", 
                    screenWidth / 2 - 250, screenHeight - 50, 20, WHITE);
        }
        
        // ===== RENDERIZAR CONFIGURAÇÕES =====
        else if (gameState == SETTINGS) {
            // Desenhar background
            Rectangle bgSrc = { 0, 0, (float)bgDarkBrick.width, (float)bgDarkBrick.height };
            Rectangle bgDst = { 0, 0, (float)screenWidth, (float)screenHeight };
            DrawTexturePro(bgDarkBrick, bgSrc, bgDst, {0, 0}, 0.0f, WHITE);
            
            // Título fixo no topo (centralizado)
            float titleScale = 2.5f;
            int titleWidth = (int)(configLabel.width * titleScale);
            int titleX = screenWidth / 2 - titleWidth / 2;
            int titleY = 30;
            
            Rectangle titleSrc = { 0, 0, (float)configLabel.width, (float)configLabel.height };
            Rectangle titleDst = { (float)titleX, (float)titleY, 
                                   (float)configLabel.width * titleScale, (float)configLabel.height * titleScale };
            DrawTexturePro(configLabel, titleSrc, titleDst, {0, 0}, 0.0f, WHITE);
            
            // Área de conteúdo com scroll
            int contentStartY = 120;
            int baseY = contentStartY - settingsScrollOffset;
            int sectionWidth = 800;
            int sectionX = (screenWidth - sectionWidth) / 2;
            float iconScale = 3.0f;
            float buttonScale = 2.5f;
            
            // ===== SEÇÃO 1: DISPLAY =====
            int section1Y = baseY;
            int section1Height = 220;
            
            // Background branco da seção
            Rectangle whiteBgSrc1 = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
            Rectangle whiteBgDst1 = { (float)sectionX, (float)section1Y, (float)sectionWidth, (float)section1Height };
            DrawTexturePro(whiteSquareUI, whiteBgSrc1, whiteBgDst1, {0, 0}, 0.0f, WHITE);
            
            // Borda preta
            DrawRectangleLinesEx({ (float)sectionX, (float)section1Y, (float)sectionWidth, (float)section1Height }, 4, BLACK);
            
            // Título da seção
            DrawText("DISPLAY", sectionX + sectionWidth / 2 - 60, section1Y + 15, 28, BLACK);
            
            // Linha separadora
            DrawRectangle(sectionX + 20, section1Y + 55, sectionWidth - 40, 3, BLACK);
            
            // Opção 0: Resolução
            int opt0Y = section1Y + 75;
            if (selectedSettingsOption == 0) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt0Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            DrawText("Resolucao:", sectionX + 40, opt0Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst = { (float)(sectionX + 400), (float)opt0Y + 5, 
                                  (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc, minusDst, {0, 0}, 0.0f, selectedSettingsOption == 0 ? WHITE : GRAY);
            
            DrawText(resolutions[currentResolution], sectionX + 500, opt0Y + 10, 22, BLACK);
            
            // Botão Plus
            Rectangle plusSrc = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst = { (float)(sectionX + 680), (float)opt0Y + 5, 
                                 (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc, plusDst, {0, 0}, 0.0f, selectedSettingsOption == 0 ? WHITE : GRAY);
            
            // Opção 1: Fullscreen
            int opt1Y = opt0Y + 70;
            if (selectedSettingsOption == 1) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt1Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            DrawText("Fullscreen:", sectionX + 40, opt1Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc1 = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst1 = { (float)(sectionX + 400), (float)opt1Y + 5, 
                                   (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc1, minusDst1, {0, 0}, 0.0f, selectedSettingsOption == 1 ? WHITE : GRAY);
            
            Texture2D fullscreenOption = isFullscreen ? yesOption : noOption;
            Rectangle fsSrc = { 0, 0, (float)fullscreenOption.width, (float)fullscreenOption.height };
            int fsWidth = (int)(fullscreenOption.width * 2.8f);
            Rectangle fsDst = { (float)(sectionX + 400 + (280 - fsWidth) / 2), (float)opt1Y + 2, 
                               (float)fullscreenOption.width * 2.8f, (float)fullscreenOption.height * 2.8f };
            DrawTexturePro(fullscreenOption, fsSrc, fsDst, {0, 0}, 0.0f, WHITE);
            
            // Botão Plus
            Rectangle plusSrc1 = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst1 = { (float)(sectionX + 680), (float)opt1Y + 5, 
                                  (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc1, plusDst1, {0, 0}, 0.0f, selectedSettingsOption == 1 ? WHITE : GRAY);
            
            // ===== SEÇÃO 2: MUSICA =====
            int section2Y = section1Y + section1Height + 40;
            int section2Height = 220;
            
            // Background branco
            Rectangle whiteBgSrc2 = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
            Rectangle whiteBgDst2 = { (float)sectionX, (float)section2Y, (float)sectionWidth, (float)section2Height };
            DrawTexturePro(whiteSquareUI, whiteBgSrc2, whiteBgDst2, {0, 0}, 0.0f, WHITE);
            
            // Borda preta
            DrawRectangleLinesEx({ (float)sectionX, (float)section2Y, (float)sectionWidth, (float)section2Height }, 4, BLACK);
            
            // Título da seção com ícone
            Rectangle musicIconSrc = { 0, 0, (float)musicLabel.width, (float)musicLabel.height };
            Rectangle musicIconDst = { (float)(sectionX + sectionWidth / 2 - 80), (float)(section2Y + 10), 
                                      (float)musicLabel.width * 2.0f, (float)musicLabel.height * 2.0f };
            DrawTexturePro(musicLabel, musicIconSrc, musicIconDst, {0, 0}, 0.0f, BLACK);
            DrawText("MUSICA", sectionX + sectionWidth / 2 - 10, section2Y + 18, 26, BLACK);
            
            // Linha separadora
            DrawRectangle(sectionX + 20, section2Y + 55, sectionWidth - 40, 3, BLACK);
            
            // Opção 2: Volume da Música
            int opt2Y = section2Y + 75;
            if (selectedSettingsOption == 2) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt2Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            Rectangle volIconSrc = { 0, 0, (float)volumeLabel.width, (float)volumeLabel.height };
            Rectangle volIconDst = { (float)(sectionX + 40), (float)opt2Y + 5, 
                                    (float)volumeLabel.width * 2.0f, (float)volumeLabel.height * 2.0f };
            DrawTexturePro(volumeLabel, volIconSrc, volIconDst, {0, 0}, 0.0f, BLACK);
            DrawText(":", sectionX + 100, opt2Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc2 = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst2 = { (float)(sectionX + 400), (float)opt2Y + 5, 
                                   (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc2, minusDst2, {0, 0}, 0.0f, selectedSettingsOption == 2 ? WHITE : GRAY);
            
            // Barra de volume com dots (sempre 6 slots, preenche com dotColor conforme nível)
            int dotStartX = sectionX + 490;
            float dotScale = 2.0f;
            int musicVolumeDots = musicVolumeLevel; // Usar níveis diretos (0 a 6)
            for (int i = 0; i < 6; i++) {
                Texture2D dot = (i < musicVolumeDots) ? dotColor : dotEmpty;
                Rectangle dotSrc = { 0, 0, (float)dot.width, (float)dot.height };
                Rectangle dotDst = { (float)(dotStartX + i * 30), (float)opt2Y + 8, 
                                    (float)dot.width * dotScale, (float)dot.height * dotScale };
                DrawTexturePro(dot, dotSrc, dotDst, {0, 0}, 0.0f, WHITE);
            }
            
            // Botão Plus
            Rectangle plusSrc2 = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst2 = { (float)(sectionX + 680), (float)opt2Y + 5, 
                                  (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc2, plusDst2, {0, 0}, 0.0f, selectedSettingsOption == 2 ? WHITE : GRAY);
            
            // Opção 3: Mute Música
            int opt3Y = opt2Y + 70;
            if (selectedSettingsOption == 3) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt3Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            Rectangle muteIconSrc = { 0, 0, (float)muteLabel.width, (float)muteLabel.height };
            Rectangle muteIconDst = { (float)(sectionX + 40), (float)opt3Y + 5, 
                                     (float)muteLabel.width * 2.0f, (float)muteLabel.height * 2.0f };
            DrawTexturePro(muteLabel, muteIconSrc, muteIconDst, {0, 0}, 0.0f, BLACK);
            DrawText(":", sectionX + 100, opt3Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc3 = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst3 = { (float)(sectionX + 400), (float)opt3Y + 5, 
                                   (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc3, minusDst3, {0, 0}, 0.0f, selectedSettingsOption == 3 ? WHITE : GRAY);
            
            Texture2D musicMuteOption = musicMuted ? yesOption : noOption;
            Rectangle mmSrc = { 0, 0, (float)musicMuteOption.width, (float)musicMuteOption.height };
            int mmWidth = (int)(musicMuteOption.width * 2.8f);
            Rectangle mmDst = { (float)(sectionX + 400 + (280 - mmWidth) / 2), (float)opt3Y + 2, 
                               (float)musicMuteOption.width * 2.8f, (float)musicMuteOption.height * 2.8f };
            DrawTexturePro(musicMuteOption, mmSrc, mmDst, {0, 0}, 0.0f, WHITE);
            
            // Botão Plus
            Rectangle plusSrc3 = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst3 = { (float)(sectionX + 680), (float)opt3Y + 5, 
                                  (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc3, plusDst3, {0, 0}, 0.0f, selectedSettingsOption == 3 ? WHITE : GRAY);
            
            // ===== SEÇÃO 3: EFEITOS SONOROS =====
            int section3Y = section2Y + section2Height + 40;
            int section3Height = 220;
            
            // Background branco
            Rectangle whiteBgSrc3 = { 0, 0, (float)whiteSquareUI.width, (float)whiteSquareUI.height };
            Rectangle whiteBgDst3 = { (float)sectionX, (float)section3Y, (float)sectionWidth, (float)section3Height };
            DrawTexturePro(whiteSquareUI, whiteBgSrc3, whiteBgDst3, {0, 0}, 0.0f, WHITE);
            
            // Borda preta
            DrawRectangleLinesEx({ (float)sectionX, (float)section3Y, (float)sectionWidth, (float)section3Height }, 4, BLACK);
            
            // Título da seção com ícone
            Rectangle soundIconSrc = { 0, 0, (float)soundLabel.width, (float)soundLabel.height };
            Rectangle soundIconDst = { (float)(sectionX + sectionWidth / 2 - 80), (float)(section3Y + 10), 
                                      (float)soundLabel.width * 2.0f, (float)soundLabel.height * 2.0f };
            DrawTexturePro(soundLabel, soundIconSrc, soundIconDst, {0, 0}, 0.0f, BLACK);
            DrawText("SFX", sectionX + sectionWidth / 2 + 10, section3Y + 18, 26, BLACK);
            
            // Linha separadora
            DrawRectangle(sectionX + 20, section3Y + 55, sectionWidth - 40, 3, BLACK);
            
            // Opção 4: Volume SFX
            int opt4Y = section3Y + 75;
            if (selectedSettingsOption == 4) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt4Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            Rectangle volIconSrc4 = { 0, 0, (float)volumeLabel.width, (float)volumeLabel.height };
            Rectangle volIconDst4 = { (float)(sectionX + 40), (float)opt4Y + 5, 
                                     (float)volumeLabel.width * 2.0f, (float)volumeLabel.height * 2.0f };
            DrawTexturePro(volumeLabel, volIconSrc4, volIconDst4, {0, 0}, 0.0f, BLACK);
            DrawText(":", sectionX + 100, opt4Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc4 = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst4 = { (float)(sectionX + 400), (float)opt4Y + 5, 
                                   (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc4, minusDst4, {0, 0}, 0.0f, selectedSettingsOption == 4 ? WHITE : GRAY);
            
            // Barra de volume SFX (sempre 6 slots, preenche com dotColor conforme nível)
            int sfxVolumeDots = sfxVolumeLevel; // Usar níveis diretos (0 a 6)
            for (int i = 0; i < 6; i++) {
                Texture2D dot = (i < sfxVolumeDots) ? dotColor : dotEmpty;
                Rectangle dotSrc = { 0, 0, (float)dot.width, (float)dot.height };
                Rectangle dotDst = { (float)(dotStartX + i * 30), (float)opt4Y + 8, 
                                    (float)dot.width * dotScale, (float)dot.height * dotScale };
                DrawTexturePro(dot, dotSrc, dotDst, {0, 0}, 0.0f, WHITE);
            }
            
            // Botão Plus
            Rectangle plusSrc4 = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst4 = { (float)(sectionX + 680), (float)opt4Y + 5, 
                                  (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc4, plusDst4, {0, 0}, 0.0f, selectedSettingsOption == 4 ? WHITE : GRAY);
            
            // Opção 5: Mute SFX
            int opt5Y = opt4Y + 70;
            if (selectedSettingsOption == 5) {
                DrawRectangleRec({ (float)sectionX + 20, (float)opt5Y - 5, (float)sectionWidth - 40, 50 }, Fade(YELLOW, 0.3f));
            }
            Rectangle muteIconSrc5 = { 0, 0, (float)muteLabel.width, (float)muteLabel.height };
            Rectangle muteIconDst5 = { (float)(sectionX + 40), (float)opt5Y + 5, 
                                      (float)muteLabel.width * 2.0f, (float)muteLabel.height * 2.0f };
            DrawTexturePro(muteLabel, muteIconSrc5, muteIconDst5, {0, 0}, 0.0f, BLACK);
            DrawText(":", sectionX + 100, opt5Y + 10, 24, BLACK);
            
            // Botão Minus
            Rectangle minusSrc5 = { 0, 0, (float)minusButton.width, (float)minusButton.height };
            Rectangle minusDst5 = { (float)(sectionX + 400), (float)opt5Y + 5, 
                                   (float)minusButton.width * buttonScale, (float)minusButton.height * buttonScale };
            DrawTexturePro(minusButton, minusSrc5, minusDst5, {0, 0}, 0.0f, selectedSettingsOption == 5 ? WHITE : GRAY);
            
            Texture2D sfxMuteOption = sfxMuted ? yesOption : noOption;
            Rectangle smSrc = { 0, 0, (float)sfxMuteOption.width, (float)sfxMuteOption.height };
            int smWidth = (int)(sfxMuteOption.width * 2.8f);
            Rectangle smDst = { (float)(sectionX + 400 + (280 - smWidth) / 2), (float)opt5Y + 2, 
                               (float)sfxMuteOption.width * 2.8f, (float)sfxMuteOption.height * 2.8f };
            DrawTexturePro(sfxMuteOption, smSrc, smDst, {0, 0}, 0.0f, WHITE);
            
            // Botão Plus
            Rectangle plusSrc5 = { 0, 0, (float)plusButton.width, (float)plusButton.height };
            Rectangle plusDst5 = { (float)(sectionX + 680), (float)opt5Y + 5, 
                                  (float)plusButton.width * buttonScale, (float)plusButton.height * buttonScale };
            DrawTexturePro(plusButton, plusSrc5, plusDst5, {0, 0}, 0.0f, selectedSettingsOption == 5 ? WHITE : GRAY);
            
            // Instruções fixas no rodapé
            DrawRectangle(0, screenHeight - 80, screenWidth, 80, Fade(BLACK, 0.8f));
            DrawText("SETAS: Navegar | ESQUERDA/DIREITA: Ajustar | Q/BACKSPACE: Voltar", 
                    screenWidth / 2 - 320, screenHeight - 50, 16, WHITE);
        }
        
        // ===== RENDERIZAR MENU DE PAUSA =====
        else if (gameState == PAUSE_MENU) {
            // Desenhar jogo com overlay escuro
            ClearBackground(DARKGRAY);
            
            // Overlay semi-transparente
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
            
            // Título
            DrawText("PAUSADO", screenWidth / 2 - 100, 100, 50, WHITE);
            
            // Opções do menu de pausa
            const char* pauseOptions[] = { "CONTINUAR", "CONFIGURACOES", "SAIR DA PARTIDA", "SAIR DO JOGO" };
            int startY = 250;
            int spacing = 60;
            
            for (int i = 0; i < 4; i++) {
                Color textColor = (i == selectedPauseOption) ? YELLOW : WHITE;
                int fontSize = (i == selectedPauseOption) ? 30 : 25;
                int textWidth = MeasureText(pauseOptions[i], fontSize);
                
                if (i == selectedPauseOption) {
                    DrawText(">", screenWidth / 2 - textWidth / 2 - 30, startY + (i * spacing), fontSize, YELLOW);
                }
                
                DrawText(pauseOptions[i], screenWidth / 2 - textWidth / 2, startY + (i * spacing), fontSize, textColor);
            }
            
            // Instruções
            DrawText("Use SETAS para navegar | ENTER para selecionar | P para continuar", 
                    screenWidth / 2 - 320, screenHeight - 40, 16, LIGHTGRAY);
        }
        
        // ===== RENDERIZAR INVENTÁRIO =====
        else if (gameState == INVENTORY) {
            // Desenhar jogo de fundo com overlay escuro
            ClearBackground(DARKGRAY);
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
            
            // Título
            DrawText("INVENTARIO", screenWidth / 2 - 110, 50, 40, WHITE);
            
            // Área central do inventário
            int invWidth = 600;
            int invHeight = 400;
            int invX = (screenWidth - invWidth) / 2;
            int invY = 150;
            
            // Background do inventário
            Rectangle invBgSrc = { 0, 0, (float)brownSquaresUI.width, (float)brownSquaresUI.height };
            Rectangle invBgDst = { (float)invX, (float)invY, (float)invWidth, (float)invHeight };
            DrawTexturePro(brownSquaresUI, invBgSrc, invBgDst, {0, 0}, 0.0f, WHITE);
            DrawRectangleLinesEx({ (float)invX, (float)invY, (float)invWidth, (float)invHeight }, 4, BLACK);
            
            // Grid de 8 slots (2 linhas x 4 colunas) - Primeiros 4 slots são para munições
            int slotSize = 100;
            int slotSpacing = 20;
            int gridStartX = invX + (invWidth - (4 * slotSize + 3 * slotSpacing)) / 2;
            int gridStartY = invY + (invHeight - (2 * slotSize + slotSpacing)) / 2;
            
            for (int row = 0; row < 2; row++) {
                for (int col = 0; col < 4; col++) {
                    int slotIndex = row * 4 + col;
                    int slotX = gridStartX + col * (slotSize + slotSpacing);
                    int slotY = gridStartY + row * (slotSize + slotSpacing);
                    
                    // Cor do slot baseado em seleção e equipamento
                    Color slotColor = LIGHTGRAY;
                    Color borderColor = DARKGRAY;
                    
                    if (slotIndex == selectedInventorySlot) {
                        borderColor = YELLOW; // Slot selecionado
                    }
                    
                    if (slotIndex == equippedAmmo) {
                        borderColor = GREEN; // Munição equipada
                    }
                    
                    // Desenhar slot (dotEmpty como fundo)
                    Rectangle slotSrc = { 0, 0, (float)dotEmpty.width, (float)dotEmpty.height };
                    Rectangle slotDst = { (float)slotX, (float)slotY, (float)slotSize, (float)slotSize };
                    DrawTexturePro(dotEmpty, slotSrc, slotDst, {0, 0}, 0.0f, slotColor);
                    DrawRectangleLinesEx({ (float)slotX, (float)slotY, (float)slotSize, (float)slotSize }, 3, borderColor);
                    
                    // Número do slot
                    DrawText(TextFormat("%d", slotIndex + 1), slotX + 5, slotY + 5, 18, GRAY);
                    
                    // Desenhar item se houver (primeiros 4 slots = munições)
                    if (slotIndex < 4 && playerAmmoInventory[slotIndex] != -1) {
                        int ammoItemIndex = playerAmmoInventory[slotIndex];
                        AmmoItem& item = ammoItems[ammoItemIndex];
                        
                        // Desenhar sprite da munição
                        float itemScale = 0.6f;
                        Rectangle itemSrc = { 0, 0, (float)item.texture.width, (float)item.texture.height };
                        Rectangle itemDst = { (float)(slotX + slotSize / 2), (float)(slotY + slotSize / 2),
                                             item.texture.width * itemScale, item.texture.height * itemScale };
                        Vector2 itemOrigin = { itemDst.width / 2, itemDst.height / 2 };
                        DrawTexturePro(item.texture, itemSrc, itemDst, itemOrigin, 0.0f, WHITE);
                        
                        // Mostrar indicador "E" se equipada
                        if (slotIndex == equippedAmmo) {
                            DrawText("E", slotX + slotSize - 20, slotY + slotSize - 20, 16, GREEN);
                        }
                    }
                }
            }
            
            // Informações da munição selecionada
            if (selectedInventorySlot >= 0 && selectedInventorySlot < 4 && playerAmmoInventory[selectedInventorySlot] != -1) {
                int ammoItemIndex = playerAmmoInventory[selectedInventorySlot];
                AmmoItem& selectedItem = ammoItems[ammoItemIndex];
                
                DrawText(TextFormat("Nome: %s", selectedItem.name), invX + 20, invY + invHeight + 30, 20, selectedItem.color);
                DrawText(TextFormat("Dano: %.1fx", selectedItem.damageMultiplier), invX + 20, invY + invHeight + 55, 18, WHITE);
                
                if (selectedInventorySlot == equippedAmmo) {
                    DrawText("[EQUIPADA]", invX + 250, invY + invHeight + 40, 20, GREEN);
                } else {
                    DrawText("Pressione ENTER para equipar", invX + 250, invY + invHeight + 40, 18, YELLOW);
                }
            } else {
                DrawText("Derrote bosses para obter municoes especiais!", 
                        screenWidth / 2 - 220, invY + invHeight + 30, 20, LIGHTGRAY);
            }
            
            // Instruções
            DrawText("SETAS para navegar | ENTER para equipar | ESC desequipa | I/B para fechar", 
                    screenWidth / 2 - 330, screenHeight - 50, 18, WHITE);
        }
        
        // ===== RENDERIZAR MENU DE CHEATS =====
        else if (gameState == CHEAT_MENU) {
            // Desenhar fundo escuro
            ClearBackground(BLACK);
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.95f));
            
            // Título
            DrawText("CHEAT MENU", screenWidth / 2 - 100, 100, 40, RED);
            
            // Opções
            const char* cheatOptions[] = {
                "Add 100,000 Coins",
                "Spawn Boss"
            };
            
            int startY = 250;
            int spacing = 80;
            
            for (int i = 0; i < 2; i++) {
                Color textColor = (i == selectedCheatOption) ? YELLOW : WHITE;
                int fontSize = (i == selectedCheatOption) ? 30 : 25;
                int textWidth = MeasureText(cheatOptions[i], fontSize);
                
                DrawText(cheatOptions[i], 
                        screenWidth / 2 - textWidth / 2, 
                        startY + (i * spacing), 
                        fontSize, 
                        textColor);
                
                // Indicador de seleção
                if (i == selectedCheatOption) {
                    DrawText(">", screenWidth / 2 - textWidth / 2 - 40, startY + (i * spacing), 30, YELLOW);
                }
            }
            
            // Instruções
            DrawText("SETAS para navegar | ENTER para ativar | ESC/HOME para fechar", 
                    screenWidth / 2 - 300, screenHeight - 80, 18, GRAY);
        }
        
        // ===== RENDERIZAR JOGO =====
        else if (gameState == PLAYING) {
            if (!gameOver) {
            // Iniciar modo de câmera para zoom
            BeginMode2D(camera);
            
            // Desenhar chão do mapa
            Texture2D currentGround = isBossWave ? groundBossTexture : groundTexture;
            // Desenhar o chão repetido para cobrir área visível
            for (int x = -5; x <= 5; x++) {
                for (int y = -5; y <= 5; y++) {
                    DrawTexture(currentGround, x * currentGround.width, y * currentGround.height, WHITE);
                }
            }
            
            // Desenhar jogador (sprite com rotação)
            Rectangle sourceRec = { 0, 0, (float)player.texture.width, (float)player.texture.height };
            Rectangle destRec = { player.position.x, player.position.y, (float)player.texture.width * 0.8f, (float)player.texture.height * 0.8f };
            Vector2 origin = { destRec.width / 2, destRec.height / 2 };
            DrawTexturePro(player.texture, sourceRec, destRec, origin, player.rotation, WHITE);
            
            // Desenhar círculo de colisão (debug - opcional)
            // DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius, BLUE);
            
            // Desenhar projéteis
            for (const auto& proj : projectiles) {
                if (proj.active) {
                    // Desenhar sprite da bala
                    Rectangle projSource = { 0, 0, (float)proj.texture.width, (float)proj.texture.height };
                    Rectangle projDest = { proj.position.x, proj.position.y, (float)proj.texture.width * 0.8f, (float)proj.texture.height * 0.5f };
                    Vector2 projOrigin = { projDest.width / 2, projDest.height / 2 };
                    DrawTexturePro(proj.texture, projSource, projDest, projOrigin, proj.rotation, WHITE);
                }
            }
            
            // Desenhar pickups (itens coletáveis)
            for (const auto& pickup : pickups) {
                if (pickup.active) {
                    float scale = 1.5f; // Aumentado de 0.8f para 1.5f
                    Rectangle pickupSource = { 0, 0, (float)pickup.texture.width, (float)pickup.texture.height };
                    Rectangle pickupDest = { pickup.position.x, pickup.position.y, 
                                            pickup.texture.width * scale, pickup.texture.height * scale };
                    Vector2 pickupOrigin = { pickupDest.width / 2, pickupDest.height / 2 };
                    DrawTexturePro(pickup.texture, pickupSource, pickupDest, pickupOrigin, 0.0f, WHITE);
                }
            }
            
            // Desenhar baús
            for (const auto& chest : chests) {
                if (chest.active) {
                    float chestScale = 1.2f;
                    Rectangle chestSource = { 0, 0, (float)chest.texture.width, (float)chest.texture.height };
                    Rectangle chestDest = { chest.position.x, chest.position.y,
                                           chest.texture.width * chestScale, chest.texture.height * chestScale };
                    Vector2 chestOrigin = { chestDest.width / 2, chestDest.height / 2 };
                    DrawTexturePro(chest.texture, chestSource, chestDest, chestOrigin, 0.0f, WHITE);
                    
                    // Mostrar indicador de interação quando próximo e fechado
                    float distToChest = sqrt(
                        (chest.position.x - player.position.x) * (chest.position.x - player.position.x) +
                        (chest.position.y - player.position.y) * (chest.position.y - player.position.y)
                    );
                    
                    if (distToChest < (chest.radius + player.radius + 30.0f) && chest.state == CHEST_CLOSED) {
                        DrawText("Pressione E", 
                                chest.position.x - 45, chest.position.y - chest.radius - 20, 
                                16, YELLOW);
                    }
                    
                    // Mostrar item revelado quando baú está aberto
                    if (chest.state == CHEST_OPENED && chest.itemRevealed) {
                        int itemIndex = (int)chest.itemDrop;
                        AmmoItem& item = ammoItems[itemIndex];
                        
                        // Verificar se jogador já tinha o item
                        bool isDuplicate = false;
                        int coinReward = 50 + (itemIndex * 35);
                        
                        // Verificar pela contagem antes de adicionar (se animTimer acabou de passar 0.6s)
                        // Como já foi adicionado, verificar pela quantidade de moedas ou criar flag
                        // Solução: verificar se playerAmmoInventory[itemIndex] estava true ANTES
                        // Mas como já modificamos, vamos usar lógica inversa
                        
                        // Desenhar sprite do item acima do baú
                        float itemY = chest.position.y - chest.radius - 80;
                        float itemScale = 1.0f;
                        Rectangle itemSource = { 0, 0, (float)item.texture.width, (float)item.texture.height };
                        Rectangle itemDest = { chest.position.x, itemY,
                                              item.texture.width * itemScale, item.texture.height * itemScale };
                        Vector2 itemOrigin = { itemDest.width / 2, itemDest.height / 2 };
                        DrawTexturePro(item.texture, itemSource, itemDest, itemOrigin, 0.0f, WHITE);
                        
                        // Desenhar nome do item com cor correspondente
                        const char* displayText = item.name;
                        int textWidth = MeasureText(displayText, 18);
                        DrawText(displayText, 
                                chest.position.x - textWidth / 2, 
                                itemY - 30, 
                                18, item.color);
                    }
                }
            }
            
            // Desenhar inimigos
            for (const auto& enemy : enemies) {
                if (enemy.active) {
                    // Desenhar sprite do inimigo
                    float scale = enemy.isBoss ? 1.2f : 0.7f;
                    Rectangle enemySource = { 0, 0, (float)enemy.texture.width, (float)enemy.texture.height };
                    Rectangle enemyDest = { enemy.position.x, enemy.position.y, enemy.texture.width * scale, enemy.texture.height * scale };
                    Vector2 enemyOrigin = { enemyDest.width / 2, enemyDest.height / 2 };
                    Color tint = enemy.isBoss ? PURPLE : RED;
                    DrawTexturePro(enemy.texture, enemySource, enemyDest, enemyOrigin, enemy.rotation, tint);
                    
                    // Desenhar barra de vida para todos os inimigos (sprites)
                    float healthPercent = (float)enemy.health / (float)enemy.maxHealth;
                    
                    // Selecionar sprite baseado na porcentagem de vida
                    Texture2D currentEnemyLifeSprite;
                    if (healthPercent >= 0.8f) {
                        currentEnemyLifeSprite = enemyLifeFull;
                    } else if (healthPercent >= 0.6f) {
                        currentEnemyLifeSprite = enemyLifeAlmostFull;
                    } else if (healthPercent >= 0.4f) {
                        currentEnemyLifeSprite = enemyLifeHalf;
                    } else if (healthPercent >= 0.2f) {
                        currentEnemyLifeSprite = enemyLifeEnd;
                    } else {
                        currentEnemyLifeSprite = enemyLifeEmpty;
                    }
                    
                    // Boss tem barra maior
                    if (enemy.isBoss) {
                        float bossBarScale = 3.0f; // Aumentado de 2.0 para 3.0 para comportar texto
                        float barWidth = currentEnemyLifeSprite.width * bossBarScale;
                        float barHeight = currentEnemyLifeSprite.height * bossBarScale;
                        
                        Vector2 barPos = { enemy.position.x - (barWidth / 2.0f), 
                                          enemy.position.y - enemy.radius - 60 }; // Ajustado para cima
                        
                        // Desenhar sprite de vida do boss (aumentado)
                        Rectangle bossLifeSrc = { 0, 0, (float)currentEnemyLifeSprite.width, (float)currentEnemyLifeSprite.height };
                        Rectangle bossLifeDst = { barPos.x, barPos.y, barWidth, barHeight };
                        DrawTexturePro(currentEnemyLifeSprite, bossLifeSrc, bossLifeDst, {0, 0}, 0.0f, WHITE);
                        
                        // Texto de HP dentro do sprite (centralizado, tamanho maior)
                        const char* hpText = TextFormat("%d/%d", enemy.health, enemy.maxHealth);
                        int textWidth = MeasureText(hpText, 22);
                        DrawText(hpText, (int)(barPos.x + (barWidth - textWidth) / 2), 
                                (int)(barPos.y + (barHeight - 22) / 2), 22, BLACK);
                    } else {
                        // Inimigos normais - barra padrão
                        Vector2 barPos = { enemy.position.x - (currentEnemyLifeSprite.width / 2.0f), 
                                          enemy.position.y - enemy.radius - 25 };
                        DrawTexture(currentEnemyLifeSprite, (int)barPos.x, (int)barPos.y, WHITE);
                    }
                }
            }
            
            // Desenhar mira do mouse (em coordenadas do mundo)
            Vector2 mousePos = GetMousePosition();
            Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);
            DrawCircleLines((int)worldMousePos.x, (int)worldMousePos.y, 10, GREEN);
            DrawLine((int)worldMousePos.x - 15, (int)worldMousePos.y, (int)worldMousePos.x + 15, (int)worldMousePos.y, GREEN);
            DrawLine((int)worldMousePos.x, (int)worldMousePos.y - 15, (int)worldMousePos.x, (int)worldMousePos.y + 15, GREEN);
            
            EndMode2D();
            
            // Desenhar UI (fora da câmera para permanecer fixo na tela)
            // Barra de vida do jogador (UI com sprites)
            float playerHealthPercent = (float)player.health / (float)player.maxHealth;
            int uiBarX = 20;
            int uiBarY = 20;
            
            // Selecionar sprite baseado na porcentagem de vida
            Texture2D currentPlayerLifeSprite;
            if (playerHealthPercent >= 0.8f) {
                currentPlayerLifeSprite = playerLifeFull;
            } else if (playerHealthPercent >= 0.6f) {
                currentPlayerLifeSprite = playerLifeAlmostFull;
            } else if (playerHealthPercent >= 0.4f) {
                currentPlayerLifeSprite = playerLifeHalf;
            } else if (playerHealthPercent >= 0.2f) {
                currentPlayerLifeSprite = playerLifeEnd;
            } else {
                currentPlayerLifeSprite = playerLifeEmpty;
            }
            
            // Desenhar sprite de vida do jogador (aumentado 5x)
            float lifeScale = 5.0f;
            Rectangle lifeSourceRec = { 0, 0, (float)currentPlayerLifeSprite.width, (float)currentPlayerLifeSprite.height };
            Rectangle lifeDestRec = { (float)uiBarX, (float)uiBarY, (float)currentPlayerLifeSprite.width * lifeScale, (float)currentPlayerLifeSprite.height * lifeScale };
            Vector2 lifeOrigin = { 0, 0 };
            DrawTexturePro(currentPlayerLifeSprite, lifeSourceRec, lifeDestRec, lifeOrigin, 0.0f, WHITE);
            
            // Texto de HP dentro do sprite (centralizado)
            const char* hpText = TextFormat("%d/%d", player.health, player.maxHealth);
            int hpTextWidth = MeasureText(hpText, 20);
            DrawText(hpText, 
                    uiBarX + (int)(currentPlayerLifeSprite.width * lifeScale / 2) - hpTextWidth / 2, 
                    uiBarY + (int)(currentPlayerLifeSprite.height * lifeScale / 2) - 10, 
                    20, BLACK);
            DrawText(hpText, 
                    uiBarX + (int)(currentPlayerLifeSprite.width * lifeScale / 2) - hpTextWidth / 2 - 1, 
                    uiBarY + (int)(currentPlayerLifeSprite.height * lifeScale / 2) - 11, 
                    20, WHITE);
            
            // HUD de munição (sprites)
            int ammoStartX = uiBarX;
            int ammoStartY = uiBarY + (int)(currentPlayerLifeSprite.height * 5.0f) + 10;
            
            // Selecionar sprite baseado na quantidade de munição
            float ammoPercent = (float)player.ammo / (float)player.maxAmmo;
            Texture2D currentAmmoSprite;
            
            if (ammoPercent >= 0.9f) {
                currentAmmoSprite = bulletsFull;
            } else if (ammoPercent >= 0.7f) {
                currentAmmoSprite = bulletsAlmostFull;
            } else if (ammoPercent >= 0.4f) {
                currentAmmoSprite = bulletsHalf;
            } else if (ammoPercent >= 0.2f) {
                currentAmmoSprite = bulletsAlmostEnd;
            } else if (ammoPercent > 0.0f) {
                currentAmmoSprite = bulletsEnd;
            } else {
                currentAmmoSprite = bulletsEmpty;
            }
            
            // Desenhar sprite de munição (aumentado 3x)
            float ammoScale = 3.0f;
            Rectangle ammoSourceRec = { 0, 0, (float)currentAmmoSprite.width, (float)currentAmmoSprite.height };
            Rectangle ammoDestRec = { (float)ammoStartX, (float)ammoStartY, (float)currentAmmoSprite.width * ammoScale, (float)currentAmmoSprite.height * ammoScale };
            Vector2 ammoOrigin = { 0, 0 };
            DrawTexturePro(currentAmmoSprite, ammoSourceRec, ammoDestRec, ammoOrigin, 0.0f, WHITE);
            
            // Texto de munição ao lado do sprite
            DrawText(TextFormat("%d/%d", player.ammo, player.maxAmmo), 
                    ammoStartX + (int)(currentAmmoSprite.width * ammoScale) + 10, ammoStartY + (int)(currentAmmoSprite.height * ammoScale / 2) - 9, 18, WHITE);
            
            // Indicador de recarga (posicionado abaixo da UI de munição)
            if (isReloading) {
                float reloadPercent = (reloadTimer / reloadDuration) * 100.0f;
                DrawText(TextFormat("RECARREGANDO... %.0f%%", reloadPercent), 
                        ammoStartX, ammoStartY + (int)(currentAmmoSprite.height * 3.0f) + 5, 14, YELLOW);
            }
            
            // Desenhar HUD de pontuação e informações
            int scoreStartY = ammoStartY + (int)(currentAmmoSprite.height * 3.0f) + 30;
            DrawText(TextFormat("Score: %d", score), 20, scoreStartY, 20, WHITE);
            DrawText(TextFormat("Wave: %d %s", wave, isBossWave ? "(BOSS!)" : ""), 20, scoreStartY + 25, 20, isBossWave ? PURPLE : WHITE);
            DrawText(TextFormat("Kills: %d/%d", enemiesKilled, isBossWave ? 1 : enemiesPerWave), 20, scoreStartY + 50, 20, WHITE);
            
            // HUD de nível, XP e moedas (lado direito superior, movido para baixo)
            int infoX = screenWidth - 200;
            int infoY = 35; // Aumentado de 20 para 35
            DrawText(TextFormat("NIVEL: %d", player.level), infoX, infoY, 22, SKYBLUE);
            DrawText(TextFormat("XP: %d/%d", player.xp, player.xpToNextLevel), infoX, infoY + 25, 18, LIGHTGRAY);
            DrawCircle(infoX + 10, infoY + 58, 8, GOLD); // Ícone de moeda
            DrawText(TextFormat("x%d", player.coins), infoX + 25, infoY + 50, 20, YELLOW);
            
            DrawText("WASD/Arrows: Move | Mouse: Aim | Click: Shoot | R: Reload | I/B: Inventario", 10, screenHeight - 30, 16, LIGHTGRAY);
            DrawText("P: Menu", screenWidth - 80, 10, 16, LIGHTGRAY);
        } else {
            // Tela de Game Over - atualizar top score
            if (score > topScore) {
                topScore = score;
            }
            
            DrawText("GAME OVER!", screenWidth / 2 - 100, screenHeight / 2 - 50, 40, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - 80, screenHeight / 2, 20, WHITE);
            DrawText(TextFormat("Wave Reached: %d", wave), screenWidth / 2 - 80, screenHeight / 2 + 30, 20, WHITE);
            DrawText("Press SPACE to restart", screenWidth / 2 - 100, screenHeight / 2 + 70, 20, LIGHTGRAY);
            DrawText("Press Q/BACKSPACE for menu", screenWidth / 2 - 120, screenHeight / 2 + 100, 20, LIGHTGRAY);
        }
        }
        
        EndDrawing();
    }
    
    // Descarregar texturas
    UnloadTexture(playerTexture);
    if (playerReloadTexture.id != 0) UnloadTexture(playerReloadTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(bulletTexture);
    UnloadTexture(groundTexture);
    UnloadTexture(groundBossTexture);
    UnloadTexture(healthPickupTexture);
    UnloadTexture(lootPickupTexture);
    UnloadTexture(bgMenu);
    UnloadTexture(bgLightBrick);
    UnloadTexture(bgDarkBrick);
    
    // Descarregar UI sprites
    UnloadTexture(playerLifeFull);
    UnloadTexture(playerLifeAlmostFull);
    UnloadTexture(playerLifeHalf);
    UnloadTexture(playerLifeEnd);
    UnloadTexture(playerLifeEmpty);
    UnloadTexture(enemyLifeFull);
    UnloadTexture(enemyLifeAlmostFull);
    UnloadTexture(enemyLifeHalf);
    UnloadTexture(enemyLifeEnd);
    UnloadTexture(enemyLifeEmpty);
    UnloadTexture(bulletsFull);
    UnloadTexture(bulletsAlmostFull);
    UnloadTexture(bulletsHalf);
    UnloadTexture(bulletsAlmostEnd);
    UnloadTexture(bulletsEnd);
    UnloadTexture(bulletsEmpty);
    
    // Descarregar UI sprites de Settings
    UnloadTexture(configLabel);
    UnloadTexture(musicLabel);
    UnloadTexture(soundLabel);
    UnloadTexture(volumeLabel);
    UnloadTexture(muteLabel);
    UnloadTexture(yesOption);
    UnloadTexture(noOption);
    UnloadTexture(dotColor);
    UnloadTexture(dotEmpty);
    UnloadTexture(frameLabel);
    UnloadTexture(whiteSquareUI);
    UnloadTexture(brownSquaresUI);
    UnloadTexture(minusButton);
    UnloadTexture(plusButton);
    
    // Descarregar texturas de munições especiais
    UnloadTexture(waterBulletTexture);
    UnloadTexture(fireBulletTexture);
    UnloadTexture(acidBulletTexture);
    UnloadTexture(magicBulletTexture);
    
    // Descarregar texturas de baú
    for (int i = 0; i < 6; i++) {
        UnloadTexture(chestTextures[i]);
    }
    
    // Descarregar músicas
    if (currentMusic != nullptr) StopMusicStream(*currentMusic);
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(levelMusic);
    UnloadMusicStream(bossMusic);
    
    // Descarregar sons SFX
    UnloadSound(defaultShootSound);
    UnloadSound(dmgSound);
    UnloadSound(deathSound);
    UnloadSound(reloadSound);
    UnloadSound(menuButtonSound);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
