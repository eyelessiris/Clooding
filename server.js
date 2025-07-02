const express = require('express');
const cors = require('cors');
const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.json());

const MAX_SELF_STUDY = 50;
const MAX_MASSAGE_CHAIR = 5;

let selfStudyApplicants = [];
let massageChairApplicants = [];

// 이미 신청했는지 체크
function alreadyApplied(applicants, studentId) {
  return applicants.some(a => a.studentId === studentId);
}

// 자습실 신청
app.post('/apply/self-study', (req, res) => {
  const { studentId, name } = req.body;
  if (!studentId || !name) {
    return res.status(400).json({ success: false, message: 'studentId와 name이 필요합니다.' });
  }
  const idNum = Number(studentId);
  if (isNaN(idNum)) {
    return res.status(400).json({ success: false, message: 'studentId는 숫자여야 합니다.' });
  }

  if (alreadyApplied(selfStudyApplicants, idNum)) {
    return res.status(400).json({ success: false, message: '이미 자습실 신청을 하셨습니다.' });
  }
  if (selfStudyApplicants.length >= MAX_SELF_STUDY) {
    return res.status(400).json({ success: false, message: '자습실 정원이 가득 찼습니다.' });
  }
  selfStudyApplicants.push({ studentId: idNum, name });
  res.json({ success: true, message: '자습실 신청이 완료되었습니다.' });
});

// 자습실 신청 취소 (DELETE)
app.delete('/apply/self-study', (req, res) => {
  const studentId = Number(req.query.studentId);
  if (!studentId || isNaN(studentId)) {
    return res.status(400).json({ success: false, message: '유효한 studentId가 필요합니다.' });
  }

  const index = selfStudyApplicants.findIndex(app => app.studentId === studentId);
  if (index === -1) {
    return res.status(404).json({ success: false, message: '신청 기록이 없습니다.' });
  }

  selfStudyApplicants.splice(index, 1);
  res.json({ success: true, message: '자습실 신청이 취소되었습니다.' });
});

// 안마의자 신청
app.post('/apply/massage-chair', (req, res) => {
  const { studentId, name } = req.body;
  if (!studentId || !name) {
    return res.status(400).json({ success: false, message: 'studentId와 name이 필요합니다.' });
  }
  const idNum = Number(studentId);
  if (isNaN(idNum)) {
    return res.status(400).json({ success: false, message: 'studentId는 숫자여야 합니다.' });
  }

  if (alreadyApplied(massageChairApplicants, idNum)) {
    return res.status(400).json({ success: false, message: '이미 안마의자 신청을 하셨습니다.' });
  }
  if (massageChairApplicants.length >= MAX_MASSAGE_CHAIR) {
    return res.status(400).json({ success: false, message: '안마의자 정원이 가득 찼습니다.' });
  }
  massageChairApplicants.push({ studentId: idNum, name });
  res.json({ success: true, message: '안마의자 신청이 완료되었습니다.' });
});

// 안마의자 신청 취소 (DELETE)
app.delete('/apply/massage-chair', (req, res) => {
  const studentId = Number(req.query.studentId);
  if (!studentId || isNaN(studentId)) {
    return res.status(400).json({ success: false, message: '유효한 studentId가 필요합니다.' });
  }

  const index = massageChairApplicants.findIndex(app => app.studentId === studentId);
  if (index === -1) {
    return res.status(404).json({ success: false, message: '신청 기록이 없습니다.' });
  }

  massageChairApplicants.splice(index, 1);
  res.json({ success: true, message: '안마의자 신청이 취소되었습니다.' });
});

// 자습실 신청자 목록 조회
app.get('/applicants/self-study', (req, res) => {
  res.json(selfStudyApplicants);
});

// 안마의자 신청자 목록 조회
app.get('/applicants/massage-chair', (req, res) => {
  res.json(massageChairApplicants);
});

app.listen(PORT, () => {
  console.log(`서버가 http://localhost:${PORT} 에서 실행 중입니다.`);
});
