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

import common from '@ohos.app.ability.common';
import resourceManager from '@ohos.resourceManager';
import {MusicItemData} from '../bean/MusicItemData'
// import { SongItem } from '../../viewmodel/SongData';
import { Logger } from './Logger';
const TAG = "SongItemBuilder"
export default class SongItemBuilder {
  private context: common.UIAbilityContext | undefined = AppStorage.Get('context');
  private realUrl?: resourceManager.RawFileDescriptor;
  private songItem: MusicItemData | null = null;

  public async build(songItem: MusicItemData): Promise<MusicItemData> {
    this.songItem = songItem;
    Logger.info(TAG,`Title: ${this.songItem.title}`)
    if (!this.context) {
      return this.songItem;
    }
    Logger.info(TAG, `song src is: ${songItem.src}`)
    let rawfileFd = await this.context.resourceManager.getRawFd(songItem.src)
      .catch((error: Error) => {
        Logger.error(`resourceManager error cause : ${JSON.stringify(error)}`);
      })
    if (rawfileFd) {
      this.realUrl = rawfileFd;
    } else {
      Logger.error('get rawfileFd failed')
    }
    Logger.info('MediaAssetBuilder build realUrl:' + this.realUrl);
    return this.songItem;
  }

  public getRealUrl(): resourceManager.RawFileDescriptor | undefined {
    return this.realUrl;
  }

  public async release(): Promise<void> {
    if (this.context && this.context !== null && this.songItem !== null) {
      this.context.resourceManager.closeRawFd(this.songItem.src);
    }
    this.songItem = null;
  }
}