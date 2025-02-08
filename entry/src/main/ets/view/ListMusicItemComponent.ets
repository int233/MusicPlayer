import { MusicItemData } from '../common/bean/MusicItemData';
import { CommonConstants } from '../common/constants/CommonConstants';
import { Logger } from '../common/utils/Logger';


/**
 * List item information component.
 */
@Component
export struct ListMusicItemComponent {
  private musicItems: MusicItemData;

  build() {
    Row() {
      Image(this.musicItems.imageAlbum)
        .objectFit(ImageFit.Contain)
        .width($r('app.float.image_star_width'))
        .height($r('app.float.image_star_height'))
        .margin({
          right: $r('app.float.list_image_margin_right')
        })
      .onClick(()=>{
        Logger.info(this.musicItems.title)
        // Logger.info(getContext(this).resourceManager.getStringValue(this.musicItems.imageAlbum).toString())

      })
      Column() {
        Text(this.musicItems.title)
          .fontSize($r('app.float.list_item_title'))
          .fontColor($r('app.color.list_item_title'))
          .maxLines(CommonConstants.LIST_TITLE_MAX_LINES)
          .textOverflow({
            overflow: TextOverflow.Ellipsis
          })
        Text(this.musicItems.titleAlbum)
          .fontSize($r('app.float.list_item_sub_title'))
          .fontColor($r('app.color.list_item_sub_title'))
          .maxLines(CommonConstants.LIST_TITLE_MAX_LINES)
          .textOverflow({
            overflow: TextOverflow.Ellipsis
          })
          .margin({
            top: $r('app.float.sub_title_margin_top')
          })
      }
      .layoutWeight(CommonConstants.LIST_TITLE_LAYOUT_WEIGHT)
      .alignItems(HorizontalAlign.Start)

      Text(this.musicItems.title)
        .fontSize($r('app.float.list_item_summary'))
        .margin({
          right: $r('app.float.list_summary_margin_right')
        })
        .fontColor($r('app.color.list_item_sub_title'))
      Image($r('app.media.ic_right_arrow'))
        .objectFit(ImageFit.Contain)
        .width($r('app.float.image_right_arrow_width'))
        .height($r('app.float.image_right_arrow_height'))
    }
    .height($r('app.float.list_item_height'))
  }
}