import { Component, OnInit } from '@angular/core';
import {Token} from '../../model/token';
import {TokenService} from '../../service/token.service';

@Component({
  selector: 'app-token-list',
  templateUrl: './token-list.component.html',
  styleUrls: ['./token-list.component.css']
})
export class TokenListComponent implements OnInit {

  tokens: Token[];

  constructor(private service: TokenService) { }

  ngOnInit() {
    this.service.getAllTokens().subscribe(
      data => this.tokens = data
    );
  }

  remove(token: Token) {
    this.service.removeToken(token);
  }

}
