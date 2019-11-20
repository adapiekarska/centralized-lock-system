import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalTokenShowComponent } from './modal-token-show.component';

describe('ModalTokenShowComponent', () => {
  let component: ModalTokenShowComponent;
  let fixture: ComponentFixture<ModalTokenShowComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalTokenShowComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalTokenShowComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
