// import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';
import { HashRouter, Routes, Route } from "react-router-dom";
import { HomePage } from "src/features/home";
import { ChaptersPage } from "src/features/chapters";
import React from "react";

export function App() {
  return (
    <HashRouter>
      <Routes>
        <Route path="/" element={<HomePage />} />
        <Route path="/chapters" element={<ChaptersPage />} />
      </Routes>
    </HashRouter>
  );
}
