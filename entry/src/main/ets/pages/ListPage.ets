import { CommonConstants } from '../common/constants/CommonConstants';
import type { TabItemData } from '../common/bean/TabItemData';
import { ListAreaComponent } from '../view/ListAreaComponent';
import PageViewModel from '../viewmodel/PageViewModel';

/**
 * ListPage is the entry of application and shows how to develop a list page.The page consists of the search component,
 * list, and tabs. The list component is ListAreaComponent, and each item in the list corresponds to ListItemComponent.
 * The LazyForEach interface is used to implement high performance. Developers can expand
 * ListItemData, TabItemData, and PageViewModel, modify or add your own text, pictures, etc,
 * and quickly develop functions that meet project requirements.
 */
@Entry
@Component
struct ListPage {
  @State currentTabIndex: number = 0;
  @State breakPoint: string = CommonConstants.BREAK_POINT_XS;

  @Builder BuildTabs(item: TabItemData, index: number) {
    Column() {
      Image(this.currentTabIndex === index ? item.imageActivated : item.imageOriginal)
        .width($r('app.float.image_star_width'))
        .height($r('app.float.image_star_height'))
      Text(item.title)
        .margin({ top: $r('app.float.tab_text_margin_top') })
        .fontSize($r('app.float.tab_text_size'))
        .fontColor(this.currentTabIndex === index ?
		  $r('app.color.tab_text_activated') : $r('app.color.tab_text_normal'))
    }
    .justifyContent(FlexAlign.Center)
    .width(CommonConstants.TABS_WIDTH_PERCENT)
    .height($r('app.float.tab_bar_height'))
  }

  build() {
    Column() {
      Tabs({
        index: this.currentTabIndex,
        barPosition: this.breakPoint === CommonConstants.BREAK_POINT_LG ? BarPosition.Start : BarPosition.End
      }) {
        ForEach(PageViewModel.getTabData(), (item: TabItemData, index) => {
          TabContent() {
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
                Column() {
                  this.Title()

                  Search({
                    placeholder: CommonConstants.SEARCH_PLACEHOLDER
                  })
                    .width(CommonConstants.SEARCH_WIDTH_PERCENT)
                    .height($r('app.float.search_height'))
                    .border({ radius: $r('app.float.search_radius') })
                    .placeholderColor($r('app.color.search_placeholder'))
                    .placeholderFont({
                      size: $r('app.float.search_text_size'),
                      weight: CommonConstants.SEARCH_FONT_WEIGHT,
                      family: CommonConstants.SEARCH_FONT_FAMILY,
                      style: FontStyle.Normal
                    })
                    .textFont({ size: $r('app.float.search_text_size') })
                    .margin({
                      bottom: $r('app.float.search_margin_bottom')
                    })

                  ListAreaComponent()
                    .layoutWeight(CommonConstants.LIST_LAYOUT_WEIGHT)
                }
                .justifyContent(FlexAlign.Center)
                .width(CommonConstants.SEARCH_WIDTH_PERCENT)
                .height(CommonConstants.COLUMN_HEIGHT_PERCENT)
              }
            }
            .margin({
              left: $r('app.float.grid_row_margin'),
              right: $r('app.float.grid_row_margin')
            })
            .onBreakpointChange((breakPoint) => {
              this.breakPoint = breakPoint;
            })
          }
          .backgroundColor($r('app.color.theme_background'))
          .tabBar(this.BuildTabs(item, index))
        }, (item, index) => index + JSON.stringify(item))
      }
      .vertical(this.breakPoint === CommonConstants.BREAK_POINT_LG)
      .barWidth(this.breakPoint === CommonConstants.BREAK_POINT_LG ?
        $r('app.float.bar_width') : CommonConstants.TAB_BAR_HEIGHT_PERCENT)
      .barHeight(this.breakPoint === CommonConstants.BREAK_POINT_LG ?
        CommonConstants.TAB_HEIGHT_PERCENT : $r('app.float.bar_height'))
      .margin({ top: $r('app.float.tab_margin_top') })
      .layoutWeight(CommonConstants.TAB_LAYOUT_WEIGHT)
      .backgroundColor($r('app.color.tab_bar_background'))
      .onChange(index => {
        this.currentTabIndex = index;
      })
    }
    .backgroundColor($r('app.color.theme_background'))
  }

  @Builder Title() {
    Row() {
      Text($r('app.string.page_title'))
        .fontColor($r('app.color.title'))
        .fontSize($r('app.float.title_size'))
    }
    .width(CommonConstants.FULL_WIDTH_PERCENT)
    .height($r('app.float.title_height'))
    .margin({
      top: $r('app.float.title_margin_top'),
      left: $r('app.float.title_margin_left')
    })
  }
}