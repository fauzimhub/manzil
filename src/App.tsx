// import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';
import { HashRouter, Routes, Route, Link } from 'react-router-dom';
import { ChaptersPage } from './ChaptersPage.tsx';
import * as versesData from './VersesData.ts';

export function App() {
  return (
    <HashRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/chapters" element={<ChaptersPage />} />
      </Routes>
    </HashRouter>
  );
}

function Home() {
  return (
    <ol>
      {versesData.surahsMetadata.map((surahMetadata, i) => (
        <li key={i}>
          <Link to={`/chapters?chapter=${i + 1}&page=1`}>
            {surahMetadata.nameTransliteration} ({surahMetadata.nameTranslation})
          </Link>
        </li>
      ))}
    </ol>
  );
}
