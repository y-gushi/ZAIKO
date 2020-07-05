#include "SearchItemNUm.h"

searchItemNum::searchItemNum(Items* itemstruct, Ctags* cs) {
    its = itemstruct;
    Cels = cs;
    Mstr = MargeaSearch();
    rootMat = nullptr;
}

searchItemNum::~searchItemNum() {

}

bool searchItemNum::searchitemNumber(UINT8* uniq,char* one,char* two,char*three,char* four,char* style) {
    Row* sr = nullptr;
    sr = Cels->rows;
    Items* Item = nullptr;
    Item = its;
    UINT8* nr = nullptr;//�������͍s�@�z��
    int result = 0;
    const char* SaT[] = { "s", "1127" };
    bool incellflag = false;
    UINT8* matchItem = nullptr;

    while (sr) {//�i�Ԍ���
        Item = its;
        while (Item) {
            if (sr->cells) {
                if (sr->cells->si) {
                    result = strcmp((const char*)sr->cells->si, (const char*)Item->itn);//�i�ԁ@�V�[�g�f�[�^�̔�r
                    if (result == 0) {
                        matchItem = Item->itn;//��v�i�΂�R�s�[
                        C* cn = nullptr;
                        cn = sr->next->cells;//�i�Ԏ��̍s
                        //�ŏI�񌟍�
                        while (cn->next) {
                            cn = cn->next;
                        }
                        inputColum = cn->col;//���ŏI��
                        changenum.ColumnIncliment(&inputColum);//�ŏI��@+1
                        nr = changenum.InttoChar(sr->next->r, &result);//���͍s
                        incolumn = changenum.ColumnNumtoArray(inputColum, &result);//�ŏI��@�z��ϊ�
                        startR = sr->next->r;//�i�Ԏ��̍s
                        incellflag = true;
                        break;
                    }
                }
            }
            Item = Item->next;
        }
        if (incellflag)
            break;
        sr = sr->next;
    }
    
    if (!incellflag)
        return false;//�i�Ԉ�v�Ȃ�
    else {
        UINT32 rowslide = startR;

        char* tv = (char*)malloc(2);
        strcpy_s(tv, 2, SaT[0]);

        size_t sstrl = strlen((const char*)style) + 1;
        char* sv = (char*)malloc(sstrl);
        strcpy_s(sv, sstrl, style);

        size_t vlen = strlen((const char*)uniq) + 1;
        char* vst = (char*)malloc(vlen);
        strcpy_s(vst, vlen, (const char*)uniq);

        F* nulf = nullptr;
        UINT8* nulsi = nullptr;
        Cels->addcelldata(nr, incolumn, (UINT8*)tv, (UINT8*)sv, (UINT8*)vst, nulf, nulsi);//�ŏ��̈��ɕύX ���C���̓��t�ǉ�
        //�T�u������ǉ�
        if (one) {
            rowslide--;
            UINT8* srow = changenum.InttoChar(rowslide, &result);

            char* tvs = (char*)malloc(2);
            strcpy_s(tvs, 2, SaT[0]);

            char* svs = (char*)malloc(sstrl);
            strcpy_s(svs, sstrl, style);

            size_t vlen = strlen((const char*)one) + 1;
            char* vsts = (char*)malloc(vlen);
            strcpy_s(vsts, vlen, (const char*)one);

            F* nfs = nullptr;
            UINT8* nsis = nullptr;

            Cels->addcelldata(srow, incolumn, (UINT8*)tvs, (UINT8*)svs, (UINT8*)vsts, nfs, nsis);
        }
        if (two) {
            rowslide--;
            UINT8* srow = changenum.InttoChar(rowslide, &result);

            char* tvs = (char*)malloc(2);
            strcpy_s(tvs, 2, SaT[0]);

            char* svs = (char*)malloc(sstrl);
            strcpy_s(svs, sstrl, style);

            size_t vlen = strlen((const char*)two) + 1;
            char* vsts = (char*)malloc(vlen);
            strcpy_s(vsts, vlen, (const char*)two);

            F* nfs = nullptr;
            UINT8* nsis = nullptr;

            Cels->addcelldata(srow, incolumn, (UINT8*)tvs, (UINT8*)svs, (UINT8*)vsts, nfs, nsis);
        }
        if (three) {
            rowslide--;
            UINT8* srow = changenum.InttoChar(rowslide, &result);

            char* tvs = (char*)malloc(2);
            strcpy_s(tvs, 2, SaT[0]);

            char* svs = (char*)malloc(sstrl);
            strcpy_s(svs, sstrl, style);

            size_t vlen = strlen((const char*)three) + 1;
            char* vsts = (char*)malloc(vlen);
            strcpy_s(vsts, vlen, (const char*)three);

            F* nfs = (F*)malloc(sizeof(F)); nfs = nullptr;
            UINT8* nsis = nullptr;

            Cels->addcelldata(srow, incolumn, (UINT8*)tvs, (UINT8*)svs, (UINT8*)vsts, nfs, nsis);
        }
        if (four) {
            rowslide--;
            UINT8* srow = changenum.InttoChar(rowslide, &result);

            char* tvs = (char*)malloc(2);
            strcpy_s(tvs, 2, SaT[0]);
            char* svs = (char*)malloc(sstrl);
            strcpy_s(svs, sstrl, style);

            size_t vlen = strlen((const char*)four) + 1;
            char* vsts = (char*)malloc(vlen);
            strcpy_s(vsts, vlen, (const char*)four);

            F* nfs= nullptr;
            UINT8* nsis = nullptr;

            Cels->addcelldata(srow, incolumn, (UINT8*)tvs, (UINT8*)svs, (UINT8*)vsts, nfs, nsis);
        }
    }

    Item = its;

    while (Item) {//item �i�Ԉ�v�@�S�J���[
        sr = Cels->rows;
        result = strcmp((const char*)matchItem, (const char*)Item->itn);//�i�ԁ@�V�[�g�f�[�^�̔�r
        if (result == 0) {
            colorsearch(sr, Item, Item->itn, style);
        }
        Item = Item->next;
    }

    return incellflag;
}

void searchItemNum::colorsearch(Row* inrow, Items* IT, UINT8* itn,char* s) {
    const char* sizetable[] = { "090","100","110","120","130","140","150","160","F" };
    const char* style[] = { "1206","28","29" };
    UINT8 middle[] = "1206";
    Row* color = inrow;// = (Row*)malloc(sizeof(Row))
    UINT8* nextColor = nullptr;
    UINT8* nextSize = nullptr;
    UINT8* nowColor = nullptr;
    UINT8* nowSize = nullptr;
    UINT8* beforeColor = nullptr;
    UINT8* beforeSize = nullptr;
    ColorAnSize* CandS;
    Items* ite = (Items*)malloc(sizeof(Items));
    ite = IT;
    bool b = false;
    bool n = false;
    int result = 0;
    UINT32 i = 0;//�Z��������
    int matchflag = 0;//������@������v����
    unitC sear;//�S�p�ϊ�

    while (color) {//�X�^�[�g�ʒu�܂ňړ�
        if (color->r == startR)
            break;
        color = color->next;
    }

    if (color && color->next) {
        color = color->next;//�i�Ԏ��̍s
    }

    while (color) {
        if (color->cells) {
            if (color->cells->si) {//������@����
                i = 0;
                while (color->cells->si[i] != '\0')//�������J�E���g
                    i++;

                //�S�p�ϊ�
                ite->col = sear.changenumber(ite->col);

                color->cells->si = sear.changeKana(color->cells->si);//���ȕϊ�
                color->cells->si = sear.changenumber(color->cells->si);

                ite->col = sear.slipNum(ite->col);//�A�������@�폜

                matchflag = Mstr.seachcharactors(ite->col, color->cells->si, i, 0);//�Z���J���[�ƃA�C�e���̔�r�@��������
                if (matchflag != -1) {
                    CandS = Mstr.splitColor(color->cells->si);//�J���[�ƃT�C�Y�̕���
                    if (CandS->color && CandS->size) {
                        //free(nowColor); free(nowSize);
                        nowColor = CandS->color; nowSize = CandS->size;
                    }
                    else {
                        nowColor = nullptr; nowSize = nullptr;
                    }
                    if (color->next) {
                        if (color->next->cells) {
                            if (color->next->cells->si) {
                                i = 0;
                                while (color->next->cells->si[i] != '\0')//�������J�E���g
                                    i++;
                                //���̐F�S�p���p�ϊ�
                                color->next->cells->si = sear.changenumber(color->next->cells->si);
                                matchflag = Mstr.seachcharactors(ite->col, color->next->cells->si, i, 0);//�Z���J���[�ƃA�C�e���̔�r�@��������
                                if (matchflag != -1) {
                                    ColorAnSize* nCandnS = Mstr.splitColor(color->next->cells->si);//�J���[�ƃT�C�Y�̕���
                                    nextColor = nCandnS->color;
                                    nextSize = nCandnS->size;
                                }
                                else {
                                    nextColor = nullptr;
                                    nextSize = nullptr;
                                }
                            }
                            else {
                                nextColor = nullptr; nextSize = nullptr;
                            }
                        }
                        else {
                            nextColor = nullptr; nextSize = nullptr;
                        }
                    }
                    else {
                        nextColor = nullptr; nextSize = nullptr;
                    }
                    if (nowColor && nowSize) {
                        result = strcmp((const char*)nowColor, (const char*)ite->col);//�J���[��r
                        if (result == 0) {//�J���[��v

                            for (int i = 0; i < 9; i++) {//�T�C�Y���[�v
                                result = strcmp((const char*)nowSize, (const char*)sizetable[i]);
                                if (result == 0) {//�T�C�Y��v
                                   
                                    UINT8* sizeMatch = nullptr;//�T�C�Y�ۑ��p
                                    switch (i) {//�Y���T�C�Y
                                    case 0:sizeMatch = ite->s90; break;
                                    case 1:sizeMatch = ite->s100; break;
                                    case 2:sizeMatch = ite->s110; break;
                                    case 3:sizeMatch = ite->s120; break;
                                    case 4:sizeMatch = ite->s130; break;
                                    case 5:sizeMatch = ite->s140; break;
                                    case 6:sizeMatch = ite->s150; break;
                                    case 7:sizeMatch = ite->s160; break;
                                    case 8:sizeMatch = ite->sF; break;
                                    default:
                                        break;
                                    }
                                    if (sizeMatch) {
                                        rootMat = addmatches(rootMat, itn, nowColor);//�J���[�A�T�C�Y��v�ŕۑ�

                                        UINT8* nr = changenum.InttoChar(color->r, &result);

                                        size_t sstrl = strlen((const char*)s) + 1;
                                        char* sv = (char*)malloc(sstrl);
                                        strcpy_s(sv, sstrl, s);

                                        UINT8* TV = nullptr;
                                        UINT8* SVI= nullptr;
                                        F* FV = nullptr;
                                        Cels->addcelldata(nr, incolumn, TV, (UINT8*)sv, sizeMatch, FV, SVI);

                                    }
                                }
                            }
                            free(nextColor); free(nextSize);
                        }
                    }
                }
            }
            if (color->cells->si) {
                beforeColor = nowColor;
                beforeSize = nowSize;
            }
            else {
                //free(beforeColor); free(beforeSize);
                beforeColor = nullptr;
                beforeSize = nullptr;
            }
        }
        color = color->next;
    }
}

MatchColrs* searchItemNum::addmatches(MatchColrs* m, UINT8* i, UINT8* c)
{
    if (!m) {
        m = (MatchColrs*)malloc(sizeof(MatchColrs));
        if (m) {
            m->itemnum = i;
            m->color = c;
            m->next = nullptr;
        }
    }
    else {
        m->next = addmatches(m->next, i, c);
    }
    return m;
}

Items* searchItemNum::addItems(Items* base, Items* itm)
{
    if (!base) {
        base = (Items*)malloc(sizeof(Items));
        if (base) {
            base->itn = itm->itn;
            base->col = itm->col;
            base->next = nullptr;
        }
    }
    else {
        base->next = addItems(base->next, itm);
    }

    return base;
}