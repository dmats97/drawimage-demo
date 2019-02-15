//スプライトシートから画像を描画する
//2019/02/15 Dai Matsmoto

#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

  //各種ライブラリ初期化
  romfsInit();
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();

  //レンダリングターゲット
  C3D_RenderTarget* target;
  target = C2D_CreateScreenTarget(GFX_TOP   , GFX_LEFT);
  C3D_RenderTarget* bottom;
  bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  //テキストバッファ
  C2D_TextBuf g_Buffer;
  C2D_Text g_Text;
  g_Buffer = C2D_TextBufNew(256);
  C2D_TextParse(&g_Text, g_Buffer, "スプライトシート内のイメージデータを\n画像として描画するテスト");
  C2D_TextOptimize(&g_Text);

  //スプライトシート
  static C2D_SpriteSheet sheet;
  sheet = C2D_SpriteSheetLoad("romfs:/images.t3x");
  if (!sheet) svcBreak(USERBREAK_PANIC);
  
  u8 index = 0;

  while (aptMainLoop()){
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) break;
    C2D_Image Image = C2D_SpriteSheetGetImage(sheet, index);	

    if (kDown & KEY_LEFT  && index>0)  index--;
    if (kDown & KEY_RIGHT && index<(C2D_SpriteSheetCount(sheet)-1) ) index++;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_SceneBegin(target);
    C2D_TargetClear(target, C2D_Color32(0xA0, 0xA0, 0xFF, 0xFF));
    C2D_DrawImageAt(Image, 0.0f, 0.0f, 0.0f, NULL, 1.0f, 1.0f);
    C2D_SceneBegin(bottom);
    C2D_TargetClear(bottom, C2D_Color32(0xA0, 0xA0, 0xFF, 0xFF));
    C2D_DrawText(&g_Text, 0, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);
    C3D_FrameEnd(0);

    gspWaitForVBlank();
  }


 
  return 0;
}
