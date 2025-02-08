import router from '@ohos.router';
import { DetailListComponent } from '../view/DetailListComponent';
import { CommonConstants } from '../common/constants/CommonConstants';
import { PlayControler } from '../view/PlayControler';

/**
 * Detail page to display more information.
 */
@Entry
@Component
struct DetailPage {
  private titleParam: Resource = $r('app.string.detail_default_title');
  private swiperController: SwiperController = new SwiperController();

  aboutToAppear() {
    if (router.getParams()) {
      this.titleParam = router.getParams()[CommonConstants.KEY_PARAM_DATA] as Resource;
    }
  }

  build() {
    Column() {
      this.Title()

      GridRow({
        breakpoints: {
          value: CommonConstants.BREAK_POINTS_VALUE,
          reference: BreakpointsReference.WindowSize
        },
        columns: {
          sm: CommonConstants.COLUMN_SM,
          md: CommonConstants.COLUMN_MD,
          lg: CommonConstants.COLUMN_LG
        },
        gutter: { x: CommonConstants.GUTTER_X },
        direction: GridRowDirection.Row
      }) {
        GridCol({
          span: {
            sm: CommonConstants.SPAN_SM,
            md: CommonConstants.SPAN_MD,
            lg: CommonConstants.SPAN_LG
          },
          offset: {
            md: CommonConstants.OFFSET_MD,
            lg: CommonConstants.OFFSET_LG
          }
        }) {
          // DetailListComponent()
          Swiper(this.swiperController) {
            List(){
              this.MusicDetail()
            }

            List(){
              this.MusicAblumLyric()
            }
            List(){
              this.MusicLyric()
            }
          }
          // .margin({ top: $r('app.float.home_swiper_margin') })
          .autoPlay(false)
          // PlayControler()
        }
        .margin({
          left: $r('app.float.detail_grid_row_margin'),
          right: $r('app.float.detail_grid_row_margin')
        })
      }
    }
    .width(CommonConstants.COLUMN_WIDTH_PERCENT)
    .height(CommonConstants.COLUMN_HEIGHT_PERCENT)
    .backgroundColor($r('app.color.detail_page_background'))
  }

  @Builder Title() {
    Row() {
      Image($r("app.media.ic_back_old"))
        .width($r('app.float.image_size'))
        .height($r('app.float.image_size'))
        .margin({
          left: $r('app.float.image_margin_left'),
          right: $r('app.float.image_margin_right')
        })
        .onClick(() => {
          router.back();
        })

      Text(this.titleParam)
        .fontColor($r('app.color.title'))
        .fontSize($r('app.float.detail_title'))
    }
    .width(CommonConstants.FULL_WIDTH_PERCENT)
    .height($r('app.float.title_height'))
  }

  @Builder MusicDetail(){
    Row() {
      Text(this.titleParam)
      Image($r("app.media.ic_back_old"))
    }
  }

  @Builder MusicAblumLyric() {
    Row() {
      Image($r("app.media.ic_back_old"))
    }
  }

  @Builder MusicLyric() {
    Row() {
      Text(this.titleParam)
    }
  }
}