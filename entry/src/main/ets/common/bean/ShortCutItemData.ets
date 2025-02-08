/**
 * Tab item data entity.
 */
export class ShortCutItemData {
  /**
   * Original image of the tabs.
   */
  imageOriginal: ResourceStr;

  /**
   * Active image of the tabs.This image is displayed when you click the tab.
   */
  imageActivated: ResourceStr;

  /**
   * Tab title.
   */
  title: ResourceStr;

  /**
   * SubTab title.
   */
  subTitle: ResourceStr;

  /**
   * stateCount： 记录数量......
   */
  stateCount: Number;

  /**
   * decImage： 用于装饰的图片......
   */
  decImage: ResourceStr | Number;

  constructor(imageOriginal: ResourceStr, imageActivated: ResourceStr, title: ResourceStr, subTitle?: ResourceStr, stateCount: Number = 0, decImage?: Resource | Number) {
    this.imageOriginal = imageOriginal
    this.imageActivated = imageActivated
    this.title = title
    this.subTitle = subTitle
    this.stateCount = stateCount
    this.decImage = decImage
  }
}