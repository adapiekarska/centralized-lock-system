import {Lock} from './lock';

export class Token {

  id: number;

  locks: Lock[];
}
