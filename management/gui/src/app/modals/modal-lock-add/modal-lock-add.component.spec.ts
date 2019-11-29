import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalLockAddComponent } from './modal-lock-add.component';

describe('ModalLockAddComponent', () => {
  let component: ModalLockAddComponent;
  let fixture: ComponentFixture<ModalLockAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalLockAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalLockAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
