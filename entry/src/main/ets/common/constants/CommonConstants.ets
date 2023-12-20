/**
 * Common constants for all features.
 */
export class CommonConstants {
  /**
   * Search font weight.
   */
  static readonly SEARCH_FONT_WEIGHT: number = 10;

  /**
   * The placeholder of search component.Currently, component interfaces do not support the Resource type.
   * Therefore, you need to define constants. The default prompt in the search box does not support
   * internationalization.
   */
  static readonly SEARCH_PLACEHOLDER: string = 'Search...';

  /**
   * The font family of search component.
   */
  static readonly SEARCH_FONT_FAMILY: string = 'serif';

  /**
   * The percentage of search component.
   */
  static readonly SEARCH_WIDTH_PERCENT: string = '100%';

  /**
   * Number of tab contents.
   */
  static readonly TAB_SIZE: number = 2;

  /**
   * Layout weight of the bottom tab.
   */
  static readonly TAB_LAYOUT_WEIGHT: number = 1;

  /**
   * Number of detail list contents.
   */
  static readonly DETAIL_PAGE_LIST_SIZE: number = 2;

  /**
   * Number of list contents.
   */
  static readonly LIST_SIZE: number = 20;

  /**
   * Layout weight of the list title.
   */
  static readonly LIST_TITLE_LAYOUT_WEIGHT: number = 1;

  /**
   * Layout weight of the list.
   */
  static readonly LIST_LAYOUT_WEIGHT: number = 1;

  /**
   * Max lines of the list title.
   */
  static readonly LIST_TITLE_MAX_LINES: number = 1;

  /**
   * Detail page url.
   */
  static readonly PAGE_NAME: string = 'pages/DetailPage';

  /**
   * Key to transfer parameters when replace pages.
   */
  static readonly KEY_PARAM_DATA: string = 'data';

  /**
   * Breakpoint xs.
   */
  static readonly BREAK_POINT_XS: string = 'xs';

  /**
   * Breakpoint sm.
   */
  static readonly BREAK_POINT_SM: string = 'sm';

  /**
   * Breakpoint md.
   */
  static readonly BREAK_POINT_MD: string = 'md';

  /**
   * Breakpoint lg.
   */
  static readonly BREAK_POINT_LG: string = 'lg';

  /**
   * The break point value.
   */
  static readonly BREAK_POINTS_VALUE: Array<string> = ['320vp', '600vp', '840vp'];

  /**
   * The number of columns for SM device.
   */
  static readonly COLUMN_SM: number = 4;

  /**
   * The number of columns for MD device.
   */
  static readonly COLUMN_MD: number = 8;

  /**
   * The number of columns for LG device.
   */
  static readonly COLUMN_LG: number = 12;

  /**
   * The number of gutter x for device.
   */
  static readonly GUTTER_X: number = vp2px(24);

  /**
   * The number of span for SM device.
   */
  static readonly SPAN_SM: number = 4;

  /**
   * The number of span for MD device.
   */
  static readonly SPAN_MD: number = 6;

  /**
   * The number of span for LG device.
   */
  static readonly SPAN_LG: number = 8;

  /**
   * The number of offset for MD device.
   */
  static readonly OFFSET_MD: number = 1;

  /**
   * The number of offset for LG device.
   */
  static readonly OFFSET_LG: number = 2;

  /**
   * The percentage of tabs component.
   */
  static readonly TABS_WIDTH_PERCENT: string = '100%';

  /**
   * The full percentage of component.
   */
  static readonly FULL_WIDTH_PERCENT: string = '100%';

  /**
   * The percentage of list width.
   */
  static readonly LIST_WIDTH_PERCENT: string = '100%';

  /**
   * The percentage of list height.
   */
  static readonly LIST_HEIGHT_PERCENT: string = '100%';

  /**
   * The percentage of column component.
   */
  static readonly COLUMN_WIDTH_PERCENT: string = '100%';

  /**
   * The percentage of height in column component.
   */
  static readonly COLUMN_HEIGHT_PERCENT: string = '100%';

  /**
   * The percentage of width in row component.
   */
  static readonly ROW_WIDTH_PERCENT: string = '100%';

  /**
   * Tab bar height.
   */
  static readonly TAB_BAR_HEIGHT_PERCENT: string = '100%';

  /**
   * Tab height.
   */
  static readonly TAB_HEIGHT_PERCENT: string = '50%';

  /**
   * Music Page Index.
   */
  static readonly INDEX_MUSIC: number = 0;

  /**
   * Music Page Index.
   */
  static readonly INDEX_MINE: number = 1;
}