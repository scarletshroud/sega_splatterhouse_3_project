IMAGE title_background "sprites/welcome_background.png" NONE
IMAGE jennifer_image "sprites/jennifer.png" BEST

SPRITE rick "sprites/rick_sheet.png" 10 10 BEST 5
SPRITE zombie_sprite "sprites/zombie.png" 9 8 BEST 5
SPRITE menu_cursor "sprites/cursor.bmp" 1 1 NONE 0
SPRITE jennifer_cries "sprites/jen-cry.png" 12 13 BEST 10

PALETTE level_palette "sprites/full_test_level_2.png"
TILESET level_tileset "sprites/full_test_level_2.png" BEST ALL 
MAP level_map "sprites/full_test_level_2.png" level_tileset BEST 0

XGM title_track "music/title_theme.vgm" -1
XGM level_track "music/level_theme.vgm" -1
XGM game_over_track "music/game_over_theme.vgm" -1

WAV bump_sound "music/bump.wav" XGM
WAV rick_bump_sound "music/bump2.wav" XGM
WAV rick_death_sound "music/rick_death.wav" XGM