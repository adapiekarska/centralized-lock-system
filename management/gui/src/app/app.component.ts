import { Component } from '@angular/core';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'gui';

  // INSTALL CORS PACKAGE
  // INSTALL BOOTSTRAP npm install bootstrap
  // npm i --save @ng-bootstrap/ng-bootstrap

  constructor(
    // private modalService: NgbModal
  ) { }

  // openFormModal() {
  //   const modalRef = this.modalService.open(FormModalComponent);
  //
  //   modalRef.result.then((result) => {
  //     console.log(result);
  //   }).catch((error) => {
  //     console.log(error);
  //   });
  // }
}
