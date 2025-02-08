/**
 * Tab item data entity.
 * 标题、艺术家、艺术家、年份、歌词和专辑封面
 */
export class MusicItemData {

  id : number;

  Artist: string;
  /**
   * Original image of the tabs.
   */
  imageAlbum: string;

  /**
   * Active image of the tabs.This image is displayed when you click the tab.
   */
  imageArtist: string;

  /**
   * Tab title.
   */
  title: string;

  /**
   * time.
   */
  time: number;

  /**
   * Tab title.
   */
  titleAlbum: string;

  /**
   * Source url.
   */
  src: string;

  /**
   * SubTab title.
   */
  year: number;

  /**
   * stateCount： 记录数量......
   */
  playCount: number;


  constructor(id: number,Artist: string, imageAlbum: string, imageArtist: string,
              title: string, time: number, src: string, year: number,
              titleAlbum?: string,  playCount?: number) {
    this.id = id
    this.Artist = Artist
    this.imageAlbum = imageAlbum
    this.imageArtist = imageArtist
    this.title = title
    this.time = time
    this.src = src
    this.year = year
    this.titleAlbum = titleAlbum
    this.playCount = playCount
  }
}