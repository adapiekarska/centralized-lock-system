import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalTokenAddComponent } from './modal-token-add.component';

describe('ModalTokenAddComponent', () => {
  let component: ModalTokenAddComponent;
  let fixture: ComponentFixture<ModalTokenAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalTokenAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalTokenAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
