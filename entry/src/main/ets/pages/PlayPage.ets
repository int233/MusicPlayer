/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import router from '@ohos.router';
import { StyleConstants } from '../common/constants/StyleConstants';
import { BreakpointType } from '../common/utils/BreakpointSystem';
// import { SongItem } from '../viewmodel/SongData';
import { MusicItemData} from '../common/bean/MusicItemData'
import { CommonConstants } from '../common/constants/CommonConstants';
import { MusicList } from '../viewmodel/MusicListViewModel';

import { ControlComponent } from '../components/ControlComponent';
import { LyricsComponent } from '../components/LyricsComponent';
import { MusicInfoComponent } from '../components/MusicInfoComponent';
import { PlayConstants } from '../common/constants/PlayConstants';
import PageViewModel from '../viewmodel/PageViewModel';
import { Logger } from '../common/utils/Logger';

const TAG = 'PlayPage'


@Entry
@Component
struct PlayPage {
  @State currentTabIndex: number = 0;
  @StorageProp('currentBreakpoint') currentBreakpoint: string = 'sm';
  @StorageLink('PlayingSong') PlayingSong: MusicItemData = PageViewModel.getMusicDataByID(1)
  @StorageLink('selectIndex') selectIndex: number = 1;

  // private songList: MusicItemData = AppStorage.Get("PlayingSong")

  aboutToAppear() {
    if (router.getParams()) {
      // this.songList = router.getParams()[CommonConstants.KEY_PARAM_DATA] as MusicItemData;
      Logger.info(TAG,this.PlayingSong.title)
      // Logger.info(this.songList.Artist.toString())
      Logger.info(TAG,`selectIndex:${this.selectIndex}`)
    }
  }



  build() {
    Stack({ alignContent: Alignment.TopStart }) {
      if (this.currentBreakpoint === PlayConstants.LG) {
        Stack({ alignContent: Alignment.Top }) {
          LyricsComponent()

          Flex({ direction: FlexDirection.Column }) {
            MusicInfoComponent()
            ControlComponent()
          }
        }
      } else {
        Flex({ direction: FlexDirection.Column }) {
          Tabs({ barPosition: BarPosition.Start, index: this.currentTabIndex }) {
            TabContent() {
              MusicInfoComponent()
            }
            .tabBar(this.TabTitle(PlayConstants.TAB_SONG, 0))

            TabContent() {
              LyricsComponent()
            }
            .tabBar(this.TabTitle(PlayConstants.TAB_LYRICS, 1))
          }
          .onChange(index => this.currentTabIndex = index)
          .vertical(false)
          .barHeight($r('app.float.fifty_six'))
          .barWidth(PlayConstants.TAB_WIDTH)
          .onClick(()=>{
            Logger.info(TAG,'Click Music TAB')
          })

          ControlComponent()
            .onClick(()=>{
              Logger.info(TAG,'Click Control TAB')
            })
        }
      }

      Image($r('app.media.ic_back_down'))
        .width($r('app.float.image_back_size'))
        .height($r('app.float.image_back_size'))
        .margin({ left: $r('app.float.twenty_four'), top: $r('app.float.image_back_margin_top') })
        .onClick(() => router.back())
    }
    .backgroundImage(this.PlayingSong.imageAlbum)
    .backgroundImageSize(ImageSize.Cover)
    .backdropBlur(PlayConstants.BLUR)
    .linearGradient({
      direction: GradientDirection.Bottom,
      colors: [
        [PlayConstants.EIGHTY_WHITE_COLOR, PlayConstants.EIGHTY_WHITE],
        [PlayConstants.NINETY_WHITE_COLOR, PlayConstants.NINETY_WHITE]
      ]
    })
    .height(StyleConstants.FULL_HEIGHT)
    .width(StyleConstants.FULL_WIDTH)
  }

  @Builder
  TabTitle(title: string | Resource, index: number) {
    Text(title)
      .fontColor(this.currentTabIndex === index ? $r('app.color.text_color') : $r('app.color.text_forty_color'))
      .fontWeight(this.currentTabIndex === index ? PlayConstants.FIVE_HUNDRED : PlayConstants.FOUR_HUNDRED)
      .fontSize(new BreakpointType({
        sm: $r('app.float.font_sixteen'),
        md: $r('app.float.font_twenty')
      }).getValue(this.currentBreakpoint))
      .border({
        width: { bottom: this.currentTabIndex === index ? $r('app.float.tab_border_width') : 0 },
        color: $r('app.color.text_color')
      })
      .padding({ bottom: $r('app.float.tab_text_padding_bottom') })
  }

  pageTransition() {
    PageTransitionEnter({ duration: PlayConstants.FIVE_HUNDRED, curve: Curve.Smooth }).slide(SlideEffect.Bottom);
    PageTransitionExit({ duration: PlayConstants.FIVE_HUNDRED, curve: Curve.Smooth }).slide(SlideEffect.Bottom);
  }
}