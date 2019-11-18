import { Component, OnInit } from '@angular/core';
import {Token} from '../../model/token';
import {ActivatedRoute, Router} from '@angular/router';
import {TokenService} from '../../service/token.service';

@Component({
  selector: 'app-token-add',
  templateUrl: './token-add.component.html',
  styleUrls: ['./token-add.component.css']
})
export class TokenAddComponent implements OnInit {

  token: Token;

  constructor(private service: TokenService, private route: ActivatedRoute, private router: Router) { }

  ngOnInit() {
    this.token = new Token();
  }

  submit() {
    this.service.createToken(this.token).subscribe(() => this.router.navigateByUrl('/tokens'));
  }
}
