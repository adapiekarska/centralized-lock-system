import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TokenAddComponent } from './token-add.component';

describe('TokenAddComponent', () => {
  let component: TokenAddComponent;
  let fixture: ComponentFixture<TokenAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ TokenAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TokenAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
