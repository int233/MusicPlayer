import router from '@ohos.router';
import { CommonConstants } from '../common/constants/CommonConstants';
import type { ShortCutItemData } from '../common/bean/ShortCutItemData';
import { ListItemComponent } from './ListItemComponent';
import PageViewModel from '../viewmodel/PageViewModel';

/**
 * List area of the main tab content.
 */
@Component
export struct MineAreaComponent {
  build() {
    List(){
      ListItem(){
        Column() {
          Text("Mine")
          Grid() {
            ForEach(PageViewModel.getShortCutData(), (item: ShortCutItemData) => {
              GridItem(){
                Column(){
                  Image(item.imageOriginal)
                  Text(item.title)
                }
              }

            })
          }
          .width(CommonConstants.LIST_WIDTH_PERCENT)
          .columnsTemplate('1fr 1fr 1fr 1fr')
          .rowsTemplate('1fr 1fr')
          .columnsGap($r('app.float.home_grid_columnsGap'))
          .rowsGap($r('app.float.home_grid_rowGap'))
          .padding({ top: $r('app.float.home_grid_padding'), bottom: $r('app.float.home_grid_padding') })
          .height($r('app.float.home_grid_height'))
          .backgroundColor(Color.White)
          .borderRadius($r('app.float.home_grid_borderRadius'))


          // .divider({
          //   strokeWidth: $r('app.float.divider_height'),
          //   color: $r('app.color.list_divider'),
          //   startMargin: $r('app.float.list_divider_start_margin'),
          //   endMargin: $r('app.float.list_divider_end_margin')
          // })

          // LazyForEach(PageViewModel.getListDataSource(), (item: ListItemData) => {
          //   ListItem() {
          //     ListItemComponent({ itemInfo: item })
          //   }
          //   .onClick(() => {
          //     router.pushUrl({
          //       url: CommonConstants.PAGE_NAME,
          //       params: {
          //         data: item.title
          //       }
          //     });
          //   })
          // }, (item, index) => index + JSON.stringify(item))
          .width(CommonConstants.COLUMN_WIDTH_PERCENT)
        }
      }
    }


  }
}