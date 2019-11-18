import {Token} from './token';
import {Lock} from './lock';

export class AuthMapping {
  id: number;

  token: Token;

  lock: Lock;
}
