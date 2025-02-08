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
// import { SongItem } from './SongData';
import { MusicItemData } from '../common/bean/MusicItemData'

// id: number, imageAlbum: ResourceStr, imageArtist: ResourceStr,
// title: ResourceStr, time: number, src: ResourceStr, year: number,
// titleAlbum?: ResourceStr,  playCount?: number
const MusicList: MusicItemData[] = [
  { id: 1, Artist: '张学友',imageAlbum: 'https://bkimg.cdn.bcebos.com/pic/d0c8a786c9177f3e6709c32ce9992cc79f3df8dcf8ce?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5MzM=,g_7,xp_5,yp_5/format,f_auto', imageArtist:'https://bkimg.cdn.bcebos.com/pic/d0c8a786c9177f3e6709c32ce9992cc79f3df8dcf8ce?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5MzM=,g_7,xp_5,yp_5/format,f_auto',
    title: '吻别', time : 40, src: 'https://int0030-winserver-2019.oss-cn-beijing.aliyuncs.com/resource/other/%E5%BC%A0%E5%AD%A6%E5%8F%8B%20-%20%E5%90%BB%E5%88%AB.flac',year: 1993,
    titleAlbum: '吻别', playCount : 1},
  { id: 2, Artist: '张学友',imageAlbum: 'https://bkimg.cdn.bcebos.com/pic/dbb44aed2e738bd43acef80ca88b87d6277ff924?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2UxMTY=,g_7,xp_5,yp_5/format,f_auto', imageArtist:'https://bkimg.cdn.bcebos.com/pic/dbb44aed2e738bd43acef80ca88b87d6277ff924?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2UxMTY=,g_7,xp_5,yp_5/format,f_auto',
    title: '遥远的她', time : 40, src: 'https://int0030-winserver-2019.oss-cn-beijing.aliyuncs.com/resource/other/%E5%BC%A0%E5%AD%A6%E5%8F%8B%20-%20%E9%81%A5%E8%BF%9C%E7%9A%84%E5%A5%B9.flac',year: 1986,
    titleAlbum: 'Amour', playCount : 1},
  { id: 3, Artist: '张学友',imageAlbum: 'https://bkimg.cdn.bcebos.com/pic/b219ebc4b74543a9822628a1e05d9d82b9014a90be01?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5Mg==,g_7,xp_5,yp_5/format,f_auto', imageArtist:'https://bkimg.cdn.bcebos.com/pic/b219ebc4b74543a9822628a1e05d9d82b9014a90be01?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5Mg==,g_7,xp_5,yp_5/format,f_auto',
    title: '天气这么热', time : 40, src: 'https://int0030-winserver-2019.oss-cn-beijing.aliyuncs.com/resource/other/%E5%BC%A0%E5%AD%A6%E5%8F%8B%20-%20%E5%A4%A9%E6%B0%94%E8%BF%99%E4%B9%88%E7%83%AD.flac',year: 2001,
    titleAlbum: '学友热', playCount : 1},
  // { id: 5, title: '可可不喜欢', singer: '名佚', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar5'), src: 'dynamic.wav', index:4 },
  // { id: 6, title: '我是UOUYGBJ', singer: '我是小树', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar6'), src: 'boisterous.wav' , index:5},
  // { id: 7, title: '好好学习', singer: '全村最帅', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar17'), src: 'dynamic.wav', index:6 },
  // { id: 8, title: '安心安心', singer: '小安安', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar8'), src: 'boisterous.wav', index:7 },
  // { id: 9, title: 'HBNJGHJHB', singer: '我是小树', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar9'), src: 'dynamic.wav', index:8 },
  // { id: 10, title: '天天向上', singer: '靓仔', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar10'), src: 'boisterous.wav', index:9 },
  // { id: 11, title: 'Notebook', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar11'), src: 'dynamic.wav', index:10},
  // { id: 12, title: '我是谁', singer: '小碗你好', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar12'), src: 'boisterous.wav', index:11 },
  // { id: 13, title: '你好吗', singer: '张三-你好我好都好', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar13'), src: 'dynamic.wav', index:12 },
  // { id: 14, title: '你在哪', singer: '不知道你是谁', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar14'), src: 'boisterous.wav', index:13 },
  // { id: 15, title: 'lovely', singer: 'Gwyu-Hjjiyabn', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar15'), src: 'dynamic.wav', index:14 },
  // { id: 16, title: '谢谢你', singer: '名佚', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar16'), src: 'boisterous.wav', index:15 },
  // { id: 17, title: '我是靓仔', singer: '我是小树', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar17'), src: 'dynamic.wav', index:16 },
  // { id: 18, title: '听我说', singer: '全村最帅', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar1'), src: 'boisterous.wav', index:17 },
  // { id: 19, title: '没什么大不了', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar2'), src: 'dynamic.wav', index:18 },
  // { id: 20, title: '其实也一样', singer: '我是小树', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar13'), src: 'boisterous.wav', index:19 },
  // { id: 21, title: '想明白', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar4'), src: 'dynamic.wav', index:20 },
  // { id: 22, title: '你懂的', singer: '小安安', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar5'), src: 'boisterous.wav', index:21 },
  // { id: 23, title: '谁了解', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar6'), src: 'dynamic.wav', index:22 },
  // { id: 24, title: '白天', singer: '小安安', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar17'), src: 'dynamic.wav', index:23 },
  // { id: 25, title: '黑夜', singer: 'Gwyu-Hjjiyabn', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar15'), src: 'boisterous.wav', index:24 },
  // { id: 26, title: '春夏秋冬', singer: '名佚', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar16'), src: 'boisterous.wav', index:25 },
  // { id: 27, title: '一年四季', singer: '我是小树', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar17'), src: 'dynamic.wav', index:26 },
  // { id: 28, title: '朝雪', singer: '全村最帅', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar12'), src: 'dynamic.wav', index:27 },
  // { id: 29, title: '暮色', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar13'), src: 'boisterous.wav', index:28 },
  // { id: 30, title: '天下', singer: '我是小树', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar14'), src: 'dynamic.wav', index:29 },
  // { id: 31, title: '勇敢', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar15'), src: 'boisterous.wav', index:30 },
  // { id: 32, title: '安明', singer: '小安安', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar16'), src: 'boisterous.wav', index:31 },
  // { id: 33, title: '心安', singer: '小安安', mark: $r('app.media.ic_vip'),
  //   label: $r('app.media.ic_avatar17'), src: 'dynamic.wav', index:32 },
  // { id: 34, title: '无归', singer: '小安安', mark: $r('app.media.ic_sq'),
  //   label: $r('app.media.ic_avatar11'), src: 'boisterous.wav', index:33 }
]

export { MusicList }