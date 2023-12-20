/**
 * Tab item data entity.
 */
export class TabItemData {
  /**
   * Original image of the tabs.
   */
  imageOriginal: Resource;

  /**
   * Active image of the tabs.This image is displayed when you click the tab.
   */
  imageActivated: Resource;

  /**
   * Tab title.
   */
  title: Resource;

  constructor(imageOriginal: Resource, imageActivated: Resource, title: Resource) {
    this.imageOriginal = imageOriginal
    this.imageActivated = imageActivated
    this.title = title
  }
}