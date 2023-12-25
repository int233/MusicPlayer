import { CommonConstants } from '../common/constants/CommonConstants';
import { ListItemData } from '../common/bean/ListItemData';
import { TabItemData } from '../common/bean/TabItemData';
import { ListDataSource } from './ListDataSource';
import { ShortCutItemData } from '../common/bean/ShortCutItemData';
import { MusicItemData } from '../common/bean/MusicItemData';
import { MusicList } from '../viewmodel/MusicListViewModel';
import { Logger } from '../common/utils/Logger'
const TAG = "PageViewModel"
/**
 * Binds data to components and provides interfaces.
 */
export class PageViewModel {
  /**
   * Get data sources required by the LazyForEach interface.
   *
   * @return {ListDataSource} new ListDataSource(listItems)
   */
  getListDataSource(): ListDataSource {
    let listItems: Array<ListItemData> = [];
    for (let i = 0; i < CommonConstants.LIST_SIZE; i++) {
      let itemInfo: ListItemData = new ListItemData();
      itemInfo.image = $r('app.media.ic_normal');
      itemInfo.title = $r('app.string.list_item_title');
      itemInfo.subTitle = $r('app.string.list_item_sub_title');
      itemInfo.summary = $r('app.string.list_item_summary');
      itemInfo.rightArrowImage = $r('app.media.ic_right_arrow');
      listItems.push(itemInfo);
    }
    return new ListDataSource(listItems);
  }

  /**
   * Get item information for the tab.
   *
   * @return {Array<TabItemInfo>} tabItems
   */
  getTabData(): Array<TabItemData> {
    let tabItems: Array<TabItemData> = [
      new TabItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'), $r('app.string.tab_title_musiclist')),
      new TabItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'), $r('app.string.tab_title_mine'))
    ];
    return tabItems;
  }

  /**
   * Get item information for the tab.
   *
   * @return {Array<ShortCutItemData>} tabItems
   */
  getShortCutData(): Array<ShortCutItemData> {
    let shortCutItems: Array<ShortCutItemData> = [
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'我喜欢', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'所有列表', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'最近', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'全部', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'全部', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'我喜欢', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'所有列表', 's',0),
      new ShortCutItemData($r('app.media.ic_normal'), $r('app.media.ic_activated'),'最近', 's',0),
    ];
    return shortCutItems;
  }

  /**
   * Get detail list Data.
   *
   * @return {Array<ListItemData>} listItems
   */
  getDetailListData(): Array<ListItemData> {
    let listItems: Array<ListItemData> = [];
    for (let i = 0; i < CommonConstants.DETAIL_PAGE_LIST_SIZE; i++) {
      let itemInfo: ListItemData = new ListItemData();
      itemInfo.title = $r('app.string.detail_page_list_title');
      itemInfo.summary = $r('app.string.list_item_summary');
      listItems.push(itemInfo);
    }
    return listItems;
  }

  /**
   * Get Music item information for the tab.
   * imageAlbum: ResourceStr, imageArtist: ResourceStr,
   * title: ResourceStr, year: Number, titleAlbum?: ResourceStr,  playCount?: Number
   * @return {Array<MusicItemData>} tabItems
   */
  getMusicData(): Array<MusicItemData> {
    let musicItems: Array<MusicItemData> = MusicList
    return musicItems;
  }

  // id: number, imageAlbum: ResourceStr, imageArtist: ResourceStr,
  // title: ResourceStr, time: number, src: ResourceStr, year: number,
  // titleAlbum?: ResourceStr,  playCount?: number
  // getMusicDemoData(): MusicItemData {
  //   let musicItems: MusicItemData = { id: 1, Artist: '张学友',imageAlbum: 'https://bkimg.cdn.bcebos.com/pic/d0c8a786c9177f3e6709c32ce9992cc79f3df8dcf8ce?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5MzM=,g_7,xp_5,yp_5/format,f_auto', imageArtist:'https://bkimg.cdn.bcebos.com/pic/d0c8a786c9177f3e6709c32ce9992cc79f3df8dcf8ce?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5MzM=,g_7,xp_5,yp_5/format,f_auto',
  //     title: '吻别', time : 40, src: 'goodbye_kiss',year: 1993,
  //     titleAlbum: '吻别', playCount : 1}
  //   return musicItems
  // }

  getMusicDataByID(id): MusicItemData {
    let musicItems: Array<MusicItemData> = this.getMusicData()
    // Logger.info(TAG,`selectIndex:${id}`)
    let musicItem = MusicList.find(item => item.id === id);
    return musicItem
  }
}

let pageViewModel = new PageViewModel();

export default pageViewModel as PageViewModel;