import versesJSON from "./assets/versesimple.json";
import surahsJSON from "./assets/surah.json";
import enWithNotesJSON from "./assets/verses_en_sam-gerrans_with-notes.json";
import arOriginalJSON from "./assets/verses_ar_original.json";

const enWithNotes = (() => {
  type Verse = { surah: number; ayah: number; text: string };

  const parsedENWithNotes = (enWithNotesJSON as [number, number, string][]).map(
    ([surah, ayah, text]) => ({ surah, ayah, text }) as Verse,
  );

  return parsedENWithNotes;
})();

const arOriginal = (() => {
  type Verse = { text: string; surah: number; ayah: number };

  const arOriginal = (arOriginalJSON as [string, number, number][]).map(
    ([surah, ayah, text]) => ({ text, surah, ayah }) as Verse,
  );

  return arOriginal;
})();

const surahsData = (() => {
  interface Surah {
    number: number;
    name_arabic: string;
    name_transliteration: string;
    name_translation: string;
    verses_count: number;
  }

  const parsedSurahs = surahsJSON as Surah[];

  return parsedSurahs;
})();

export const surahsMetadata = (() => {
  const metadata: Record<string, number>[] = [];

  surahsData.forEach((surahData, index) => {
    metadata[index] = {
      number: surahData.number,
      nameArabic: surahData.arabic,
      nameTransliteration: surahData.name_transliteration,
      nameTranslation: surahData.name_translation,
      versesCount: surahData.verses_count,
    };
  });
  return metadata;
})();

const versesData = (() => {
  interface Data {
    id: string;
    chapter: string;
    verse: string;
    english: string;
    arabic: string;
  }

  interface Verses {
    type: string;
    version: string | null;
    comment: string | null;
    name: string | null;
    database: string | null;
    data: Data[] | null;
  }

  const parsedVerses = versesJSON as Verses[];
  const parsedVersesData = parsedVerses[2]?.data;

  return parsedVersesData;
})();

const verses = (() => {
  const arabicEnglish: Record<string, string>[][] = [];

  let surahIndex: number = 0;
  let ayahIndex: number = 0;

  versesData.forEach((verseData) => {
    const currentVerseCount = surahsMetadata[surahIndex].versesCount;

    if (surahIndex === 0 || surahIndex === 8) {
      if (ayahIndex === currentVerseCount) {
        ayahIndex = 0;
        surahIndex += 1;
      }
    } else {
      if (ayahIndex === currentVerseCount + 1) {
        ayahIndex = 0;
        surahIndex += 1;
      }
    }

    if (!arabicEnglish[surahIndex]) {
      arabicEnglish[surahIndex] = [];
    }

    arabicEnglish[surahIndex][ayahIndex] = {
      arabic: verseData.arabic,
      english: verseData.english,
      number: Number(verseData.verse),
    };

    ayahIndex += 1;
  });

  return arabicEnglish;
})();

const verses2 = (() => {
  const arabicEnglish: Record<string, string>[][] = [];

  let surahIndex: number = 0;
  let ayahIndex: number = 0;

  for (let i = 0; i < arOriginal.length; i++) {
    const currentVerseCount = surahsMetadata[surahIndex].versesCount;
    if (ayahIndex === currentVerseCount) {
      ayahIndex = 0;
      surahIndex += 1;
    }

    if (!arabicEnglish[surahIndex]) {
      arabicEnglish[surahIndex] = [];
    }

    arabicEnglish[surahIndex][ayahIndex] = {
      arabic: arOriginal[i]?.text,
      english: enWithNotes[i]?.text,
      number: Number(arOriginal[i]?.ayah),
    };

    ayahIndex += 1;
  }

  return arabicEnglish;
})();

export function getVerses(chapter: number): Record<string, string>[] {
  return verses2[chapter];
}

}
