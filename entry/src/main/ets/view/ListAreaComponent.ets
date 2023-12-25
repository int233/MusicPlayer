import router from '@ohos.router';
import { CommonConstants } from '../common/constants/CommonConstants';
import type { MusicItemData } from '../common/bean/MusicItemData';
import { ListMusicItemComponent } from './ListMusicItemComponent';
import PageViewModel from '../viewmodel/PageViewModel';
import { Logger } from '../common/utils/Logger';
const TAG = 'ListAreaComponent'
/**
 * List area of the main tab content.
 */
@Component
export struct ListAreaComponent {
  @StorageLink('selectIndex') selectIndex: number = 1;

  build() {
    Column() {
      Text("Music")
      List() {
        ForEach(PageViewModel.getMusicData(), (item: MusicItemData) => {
          // Logger.info(TAG, ``);
          ListItem() {
            // ListItemComponent({ itemInfo: item })

            ListMusicItemComponent({musicItems : item})
          }
          .onClick(() => {
            AppStorage.SetOrCreate('selectIndex', item.id)
            AppStorage.SetOrCreate('PlayingSong', PageViewModel.getMusicDataByID(item.id))
            Logger.info(TAG,`selectIndex:${this.selectIndex}`)
            Logger.info(item.imageAlbum)
            Logger.info(item.Artist)
            router.pushUrl({
              url: CommonConstants.PAGE_NAME,
              params: {
                data: item
              }
            });

          })
        }, (item, index) => index + JSON.stringify(item))
      }
      .width(CommonConstants.LIST_WIDTH_PERCENT)
      .divider({
        strokeWidth: $r('app.float.divider_height'),
        color: $r('app.color.list_divider'),
        startMargin: $r('app.float.list_divider_start_margin'),
        endMargin: $r('app.float.list_divider_end_margin')
      })
    }
    .width(CommonConstants.COLUMN_WIDTH_PERCENT)
  }
}