#!/usr/bin/env python3
"""Gera docs/status.md a partir das pastas de entrega em entregas/."""

from __future__ import annotations

from datetime import datetime, timezone
from pathlib import Path

SEMANAS = [f"semana-{i:02d}" for i in range(10)]
# Pastas em entregas/ que nao sao de alunos (material compartilhado)
NAO_ALUNOS = {"dados"}
ROOT = Path(__file__).resolve().parent.parent
ENTREGAS = ROOT / "entregas"
STATUS = ROOT / "docs" / "status.md"


def descobrir_alunos() -> list[str]:
    """Retorna os nomes das pastas de alunos, em ordem alfabetica."""
    if not ENTREGAS.is_dir():
        return []
    return sorted(
        pasta.name
        for pasta in ENTREGAS.iterdir()
        if pasta.is_dir() and pasta.name not in NAO_ALUNOS
    )


def semanas_entregues(aluno_dir: Path) -> set[str]:
    """Semanas que possuem conteudo entregue."""
    entregues = set()
    for semana in SEMANAS:
        pasta = aluno_dir / semana
        if pasta.is_dir() and any(pasta.iterdir()):
            entregues.add(semana)
    return entregues


def gerar_tabela(alunos: list[str]) -> str:
    cabecalho = "| Aluno | " + " | ".join(s.replace("semana-", "S") for s in SEMANAS) + " |"
    separador = "|" + "---|" * (len(SEMANAS) + 1)
    linhas = [cabecalho, separador]
    for aluno in alunos:
        entregues = semanas_entregues(ENTREGAS / aluno)
        celulas = []
        for semana in SEMANAS:
            if semana in entregues:
                celulas.append(f"[✅](../entregas/{aluno}/{semana}/)")
            else:
                celulas.append("⬜")
        linhas.append(f"| [{aluno}](../entregas/{aluno}/) | " + " | ".join(celulas) + " |")
    return "\n".join(linhas)


def main() -> None:
    alunos = descobrir_alunos()
    agora = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
    conteudo = [
        "# Status dos Trainees",
        "",
        "Progresso de cada trainee por semana. Gerado automaticamente a partir "
        "das pastas em `entregas/`.",
        "",
    ]
    if alunos:
        conteudo.append(gerar_tabela(alunos))
    else:
        conteudo.append("_Nenhuma entrega registrada ainda._")
    conteudo += [
        "",
        "**Legenda:** ✅ entregue · ⬜ pendente",
        "",
        f"Última atualização: {agora}",
        "",
        "> Gerado por `scripts/gerar-status.py`. Não edite manualmente.",
        "",
    ]
    STATUS.write_text("\n".join(conteudo), encoding="utf-8")
    print(f"Status gerado em {STATUS.relative_to(ROOT)} ({len(alunos)} aluno(s)).")


if __name__ == "__main__":
    main()
