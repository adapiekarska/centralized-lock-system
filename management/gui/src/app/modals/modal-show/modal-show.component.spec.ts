import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalShowComponent } from './modal-show.component';

describe('LockAddModalComponent', () => {
  let component: ModalShowComponent;
  let fixture: ComponentFixture<ModalShowComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalShowComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalShowComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
