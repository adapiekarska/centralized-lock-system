import {Lock} from './lock';

export class Token {

  id: number;

  tag: string;

  locks: Lock[];
}
